#ifndef GLSL_SHADER_H
#define GLSL_SHADER_H

#include <GL/glew.h>

GLuint compile_program(const char *vertex_path, const char *fragment_path);
GLuint compile_shader(const char *shader_path, GLenum shader_type);

#endif /* GLSL_SHADER_H */

