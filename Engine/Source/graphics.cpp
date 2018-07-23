/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "graphics.h"

namespace Honey {
  Graphics* graphics = new Graphics();

  Graphics::Graphics() {
  }

  void Graphics::initialize(Window* window) {
    this->window = window;

    initializeOpenGL();
    initializeShaders();
    initializeBuffersAndGeometry();
  }

  void Graphics::initializeOpenGL() {
    // Print OpenGL version and Shading Language version
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("OpenGL Shading Language Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Enable blending, so we can use the alpha or transparency component of images.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glViewport tells OpenGL it's drawing on a square the size of the window.
    glViewport(0, 0, window->width, window->height);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      printf("Error initializing OpenGL! %d\n", error);
      exit(1);
    }
  }

  void Graphics::initializeBuffersAndGeometry() {
    // Here we set up some things that are going to be fed to the shader program.
    // First is the mvp (model-view-projection) matrix.
    mvp_matrix_id = glGetUniformLocation(shader_program, "mvp_matrix");
    // Second is the texture sampler.
    texture_sampler_id = glGetUniformLocation(shader_program, "texture_sampler");
    // Third is the color.
    color_id = glGetUniformLocation(shader_program, "color");

    // Remember, modern OpenGL works by feeding data (vertices, normals, textures, colors)
    // straight to the graphics card. Here we're telling OpenGL to prepare for an array of
    // vertices.
    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    // Here we create our rectangle texture data and pass it to the card.
    GLfloat texture_data[] = { 
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f
    };

    glGenBuffers(1, &rectangle_texture_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, rectangle_texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_data) * 4 * 2, texture_data, GL_STATIC_DRAW);

    // The model matrix stores the transform (rotation, translation, scale) of
    // one object. We start out with the identity matrix, which corresponds to
    // "no transform at all".
    model = glm::mat4(1.0);

    // We also start out with white as the color.
    color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  }

  void Graphics::initializeShaders() {
    // Why do we set our shader program at 17? No reason.
    shader_program = 17;

    std::string line;

    // The shaders are defined in shaders.h
    const char *vertex_str = vertex_shader.c_str();
    const char *fragment_str = fragment_shader.c_str();

    GLint vertex_shader = 0;
    GLint fragment_shader = 0;
    GLint status = 0;

    // Create the shaders
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load the shaders from strings in shaders.h
    glShaderSource(vertex_shader, 1, &vertex_str, NULL);
    glShaderSource(fragment_shader, 1, &fragment_str, NULL);

    // Tell OpenGL to compile the shaders into lil' shader programs.
    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    // Attach both of the shader programs.
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // If the vertex shader didn't compile, print an error and exit.
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
      char log[500];
      GLint length;
      printf("Error: failed to compile the vertex shader.\n");
      glGetShaderInfoLog(vertex_shader, 500, &length, log);
      printf("Log: %s\n", log);
      exit(1);
    }

    // If the fragment shader didn't compile, print an error and exit.
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
      char log[500];
      GLint length;
      printf("Error: failed to compile the fragment shader.\n");
      glGetShaderInfoLog(fragment_shader, 500, &length, log);
      printf("Log: %s\n", log);
      exit(1);
    }

    // Link the shader program. See, this is just like a little program inside our program.
    glLinkProgram(shader_program);

    // If the linker failed, print an error and exit.
    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
    if (!status) {
      printf("Error: failed to link the shader program object.\n");
      exit(1);
    }

    // This is important. Tell OpenGL to use this particular shader program.
    // We *could* in principle make multiple shader programs and switch between them.
    glUseProgram(shader_program);
  }

  void Graphics::pushModelMatrix() {
    // We want to be able to store model states while we mess around with more model transformations.
    // Here we have a stack to store the model.
    model_stack.push_back(model);
  }

  void Graphics::popModelMatrix() {
    // When we pop the stack, we go back to the previous model.
    model = model_stack.back();
    model_stack.pop_back();

    if (using_2d) {
      // We also need to feed this old model back to the graphics card.
      glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, glm::value_ptr(projection * model));
    }
  }

  void Graphics::translate(float x, float y, float z) {
    // GLM has a method for us to translate objects. This changes the model matrix,
    // through multiplication, as though we applied a translation, ie, moved the object
    // by x, y, and z.
    // Once we've done this, we need to feed the result to the graphics card.
    model = model * glm::translate(glm::vec3(x, y, z));
    if (using_2d) {
      glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, glm::value_ptr(projection * model));
    }
  }

  void Graphics::rotate(float angle, float x, float y, float z) {
    // GLM has a method for us to rotate objects. This changes the model matrix,
    // through multiplication, as though we applied a rotation, ie, rotated the object
    // by angle degrees around the vector x, y, z.
    model = model * glm::rotate(glm::radians(angle), glm::vec3(x, y, z));
    if (using_2d) {
      glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, glm::value_ptr(projection * model));
    }
  }
  
  void Graphics::scale(float x, float y, float z) {
    // GLM has a method for us to scale objects. This changes the model matrix,
    // through multiplication, as though we applied a scale, ie, scaled the object
    // by factors of x, y, and z in those respective directions.
    model = model * glm::scale(glm::vec3(x, y, z));
    if (using_2d) {
      glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, glm::value_ptr(projection * model));
    }
  }

  void Graphics::setColor(std::string color, float opacity) {
    // The clear screen method takes a hex-string color (eg #A4F4E3 or #FFFFFF or #003030)
    // and decomposes it into r, g, and b floats, which are each a fraction from 0 (black)
    // to 1 (fully saturated). (1,0,0) is full red, (0,1,0) is full green, (0,0,1) is full
    // blue, and (1,1,1) is full white.
    float r = std::stoi(color.substr(1,2), 0, 16) / 255.0f;
    float g = std::stoi(color.substr(3,2), 0, 16) / 255.0f;
    float b = std::stoi(color.substr(5,2), 0, 16) / 255.0f;

    setColor(r, g, b, opacity);
  }

  void Graphics::setColor(float r, float g, float b, float opacity) {
    color = glm::vec4(r, g, b, opacity);
    glUniform4fv(color_id, 1, glm::value_ptr(color));
  }

  void Graphics::clearScreen(std::string color) {
    // The clear screen method takes a hex-string color (eg #A4F4E3 or #FFFFFF or #003030)
    // and decomposes it into r, g, and b floats, which are each a fraction from 0 (black)
    // to 1 (fully saturated). (1,0,0) is full red, (0,1,0) is full green, (0,0,1) is full
    // blue, and (1,1,1) is full white.
    float r = std::stoi(color.substr(1,2), 0, 16) / 255.0f;
    float g = std::stoi(color.substr(3,2), 0, 16) / 255.0f;
    float b = std::stoi(color.substr(5,2), 0, 16) / 255.0f;

    // Tell OpenGL to clear the whole screen to our chosen color.
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Also, for convenience, reset the model matrix
    model = glm::mat4(1.0);
    if (using_2d) {
      glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, glm::value_ptr(projection * model));
    }

    // and the color
    setColor(1.0f, 1.0f, 1.0f, 1.0f);
  }

  void Graphics::draw2D() {
    // In order to do 2D graphics, as opposed to 3D, we need
    // No depth testing (checking whether one polygon is closer to the camera than another),
    glDisable(GL_DEPTH_TEST);
    // an Orthographic projection (camera is looking straight overhead, and there's no perspective correction),
    projection = glm::ortho(0.0f, (float) window->width, (float) window->height, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, glm::value_ptr(projection));
    // and a boolean to make sure other parts of the system behave in a 2d way.
    using_2d = true;
  }

  void Graphics::cacheRectangle(float width, float height) {
    std::string id = std::to_string(width) + "," + std::to_string(height);

    GLfloat vertex_data[] = { 
      0, 0, 0,
      0, height, 0,
      width, height, 0,
      width, 0, 0,
    };

    glGenBuffers(1, &vertex_buffers[id]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[id]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data) * 4 * 3, vertex_data, GL_STATIC_DRAW);
  }

  void Graphics::drawRectangle(float x_position, float y_position, float width, float height) {
    std::string id = std::to_string(width) + "," + std::to_string(height);

    // If we're missing this particular rectangle size, go ahead and cache it
    if (vertex_buffers.count(id) == 0) {
      cacheRectangle(width, height);
    }

    // 1st buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[id]);
    glVertexAttribPointer(
      0,                  // buffer number 0
      3,                  // size per item is 3 (3 points to a vertex)
      GL_FLOAT,           // of type float
      GL_FALSE,           // not normalized
      0,                  // stride (ignore me!)
      (void*)0            // array buffer offset (ignore meeee!)
    );

    // 2nd buffer : textures
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, rectangle_texture_buffer);
    glVertexAttribPointer(
      1,                  // buffer number 1
      2,                  // size per item is 2 (2 points to a texture coordinate)
      GL_FLOAT,           // of type float
      GL_FALSE,           // not normalized
      0,                  // stride (ignore me!)
      (void*)0            // array buffer offset (ignore meeee!)
    );

    pushModelMatrix();
    translate(x_position, y_position, 0);

    int size = 4;

    if (size == 4) {
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    popModelMatrix();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  }

  void Graphics::addImage(std::string label, std::string path) {
    // Don't reload the same texture multiple times.
    if (texture_map.count(label) == 1) {
      return;
    }

    // Use the SDL to load the image
    SDL_Surface* image = IMG_Load(path.c_str());
    if (image == nullptr) {
      printf("IMG_Load failed for %s with error: %s\n", path.c_str(), IMG_GetError());
      exit(1);
    }

    // Make a texture
    GLuint* texture = new GLuint[1];
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);

    // Save the texture id in a map
    texture_map[label] = texture[0];

    // Also save the width and height
    texture_widths[label] = image->w;
    texture_heights[label] = image->h;
  }

  void Graphics::addTextImage(std::string label, SDL_Surface* text_render) {
    // Make a texture
    GLuint* texture = new GLuint[1];
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_render->w, text_render->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, text_render->pixels);

    // Save the texture id in a map
    texture_map[label] = texture[0];

    // Also save the width and height
    texture_widths[label] = text_render->w;
    texture_heights[label] = text_render->h;
  }

  void Graphics::setImage(std::string label) {
    if (texture_map.count(label) != 1) {
      printf("Failed to find %s among images.\n", label.c_str());
      return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_map[label]);
    glUniform1i(texture_sampler_id, 0);
  }

  void Graphics::drawImage(std::string label, int x_position, int y_position) {
    if (texture_map.count(label) != 1) {
      printf("Failed to draw %s because it wasn't cached in images.\n", label.c_str());
      return;
    }

    setImage(label);

    int width = texture_widths[label];
    int height = texture_heights[label];

    drawRectangle(x_position, y_position, width, height);
  }

  void Graphics::drawImage(std::string label, int x_position, int y_position, bool centered, float rotation, float scale) {
    pushModelMatrix();
    
    translate(x_position, y_position, 0);
    rotate(rotation, 0, 0, 1);
    this->scale(scale, scale, scale);

    if (centered) {
      int width = texture_widths[label];
      int height = texture_heights[label];
      translate(-width / 2.0, -height / 2.0, 0);
    }

    drawImage(label, 0, 0);

    popModelMatrix();
  }

  void Graphics::destroyImage(std::string label) {
    if (texture_map.count(label) != 1) {
      printf("Failed to delete %s because it wasn't cached in images.\n", label.c_str());
      return;
    }

    glActiveTexture(GL_TEXTURE0);
    glDeleteTextures(1, &texture_map[label]);
    texture_map.erase(label);
  }

  void Graphics::updateDisplay() {
    // Take everything we've drawn since the last wipe and put it on the screen all at once.
    SDL_GL_SwapWindow(window->window);
  }
}