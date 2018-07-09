/*

  Honey
  Copyright 2018 - Matthew Carlin

  Shaders just holds Honey's vertex and fragment shaders as strings.
*/

#ifndef HONEY_SHADERS_H_
#define HONEY_SHADERS_H_

#include <string>
#include <stdlib.h>

namespace Honey {
  const std::string vertex_shader = R"(
  #version 330 core

  uniform mat4 mvp_matrix;

  layout(location = 0) in vec3 position_vector;

  void main(){
    gl_Position =  mvp_matrix * vec4(position_vector, 1);
  }
  )";

  const std::string fragment_shader = R"(
  #version 330 core

  out vec3 final_color_vector;

  void main(){
    final_color_vector = vec3(0, 0, 0.5);
  } 
  )";
}
#endif
