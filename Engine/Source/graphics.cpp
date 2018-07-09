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

    // Remember, modern OpenGL works by feeding data (vertices, normals, textures, colors)
    // straight to the graphics card. Here we're telling OpenGL to prepare for an array of
    // vertices.
    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    // The model matrix stores the transform (rotation, translation, scale) of
    // one object. We start out with the identity matrix, which corresponds to
    // "no transform at all".
    model = glm::mat4(1.0);
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

  void Graphics::drawRectangle(float x_position, float y_position, float width, float height) {
    GLfloat vertex_data[] = { 
      x_position, y_position, 0,
      x_position, y_position + height, 0,
      x_position + width, y_position + height, 0,
      x_position + width, y_position, 0,
    };

    GLuint vertex_buffer;

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data) * 4 * 3, vertex_data, GL_STATIC_DRAW);

    // 1st buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(
      0,                  // buffer number 0
      3,                  // size per item is 3 (3 points to a vertex)
      GL_FLOAT,           // of type float
      GL_FALSE,           // not normalized
      0,                  // stride (ignore me!)
      (void*)0            // array buffer offset (ignore meeee!)
    );

    int size = 4;

    if (size == 4) {
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glDisableVertexAttribArray(0);
  }

  void Graphics::updateDisplay() {
    // Take everything we've drawn since the last wipe and put it on the screen all at once.
    SDL_GL_SwapWindow(window->window);
  }
}