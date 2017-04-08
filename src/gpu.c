/* Header file */
#include <linuxboy/gpu.h>
/* C Library */
#include <stdlib.h>

const color colors[MAX_COLORS] = { {   8,  24,  32 },	/* BLACK */
				   {  52, 104,  86 },	/* DARK_GRAY */
				   { 136, 192, 112 },	/* LIGHT_GRAY */
				   { 224, 248, 208 } };	/* WHITE */

void
init_gpu(gb_gpu *gpu, int width, int height)
{
	/* Quad vertices */
	static const GLfloat vertex_data[] = { -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
					       -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
					        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
					        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
					        1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
					       -1.0f,  1.0f, 1.0f, 0.0f, 1.0f };
	
	/* Setup screen properties */
	gpu->screen_width = width;
	gpu->screen_height = height;
	gpu->screen_data = (GLubyte*)malloc(width * height * 3);
	
	/* float scale = 255.0f / width; */
	for (int i = 0; i != width * height; ++i) {
		/*
		  Testing
		gpu->screen_data[i * 3] = i % width * scale;
		gpu->screen_data[i * 3 + 1] = i % width * scale;
		gpu->screen_data[i * 3 + 2] = i % width * scale;
		*/

		gpu->screen_data[i * 3] = colors[WHITE].r;
		gpu->screen_data[i * 3 + 1] = colors[WHITE].g;
		gpu->screen_data[i * 3 + 2] = colors[WHITE].b;
	}
	
	/* Basic shaders */
	gpu->shader_program = compile_program("res/vertex.glsl", "res/fragment.glsl");
	
	/* Screen quad */
	glGenVertexArrays(1, &gpu->vao);
	glGenBuffers(1, &gpu->vbo);
	
	glBindVertexArray(gpu->vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, gpu->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
	
	/* Display texture */
	glGenTextures(1, &gpu->texture);
	glBindTexture(GL_TEXTURE_2D, gpu->texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, gpu->screen_data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void
free_gpu(gb_gpu *gpu)
{
	free(gpu->screen_data);
}

void
draw(gb_gpu *gpu)
{
	glUseProgram(gpu->shader_program);
	
	glBindTexture(GL_TEXTURE_2D, gpu->texture);
	glBindVertexArray(gpu->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUseProgram(0);
}

