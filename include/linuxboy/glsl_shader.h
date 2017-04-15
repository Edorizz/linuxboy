#ifndef LINUXBOY_GLSLSHADER_H
#define LINUXBOY_GLSLSHADER_H

#include <GL/glew.h>

GLuint compile_program(const char *vertex_path, const char *fragment_path);
GLuint compile_shader(const char *shader_path, GLenum shader_type);

#endif /* LINUXBOY_GLSLSHADER_H */
