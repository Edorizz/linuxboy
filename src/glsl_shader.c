/* Header file */
#include <linuxboy/glsl_shader.h>
/* C Library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

GLuint
compile_program(const char *vertex_path, const char *fragment_path)
{
	GLuint shader_program;
	GLuint vertex_shader, fragment_shader;
	GLint status;
	char *buffer;
	int log_length;

	/* Compile shaders */
	shader_program = glCreateProgram();
	vertex_shader = compile_shader(vertex_path, GL_VERTEX_SHADER);
	fragment_shader = compile_shader(fragment_path, GL_FRAGMENT_SHADER);

	/* Attach shaders */
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	/* Link shader program */
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);

		buffer = (char*)malloc(log_length);
		glGetProgramInfoLog(shader_program, log_length, &log_length, buffer);

		fprintf(stderr, "Failed to link shaders:\n%s", buffer);
		free(buffer);
	}

	/* Free shaders */
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}

GLuint
compile_shader(const char *shader_path, GLenum shader_type)
{
	GLuint shader;
	GLint status;
	char *buffer;
	int file_size;
	FILE *fp;

	/* Open file */
	if ((fp = fopen(shader_path, "rb")) == NULL) {
		fprintf(stderr, "Failed to open %s: %s\n", shader_path, strerror(errno));
		return 0;
	}

	/* Get file size */
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	/* Read file into buffer */
	buffer = (char*)malloc(file_size + 1);
	fread(buffer, 1, file_size, fp);
	buffer[file_size] = '\0';
	fclose(fp);

	/* Compile shader source */
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, (const GLchar* const*)&buffer, NULL);
	glCompileShader(shader);
	free(buffer);

	/* Check compilation status */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &file_size);

		buffer = (char*)malloc(file_size);
		glGetShaderInfoLog(shader, file_size, &file_size, buffer);

		fprintf(stderr, "Failed to compile shader %s:\n%s", shader_path, buffer);
		free(buffer);
	}

	return shader;
}

