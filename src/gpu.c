/* Header file */
#include <linuxboy/gpu.h>
/* C Library */
#include <stdlib.h>
#include <string.h>

const color colors[MAX_COLORS] = { { 224, 248, 208 },	/* WHITE */
				   { 136, 192, 112 },	/* LIGHT_GRAY */
				   {  52, 104,  86 },	/* DARK_GRAY */
				   {   8,  24,  32 } };	/* BLACK */

void
power_gpu(gb_gpu *gpu, int width, int height)
{
	/* Quad vertices */
	static const GLfloat vertex_data[] = { -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
					       -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
					        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
					        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
					        1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
					       -1.0f,  1.0f, 1.0f, 0.0f, 1.0f };
	
	/* Setup screen properties */
	gpu->scr_w = width;
	gpu->scr_h = height;
	gpu->scr_buf = (GLubyte*)malloc(width * height * 3);
	
	clear_screen(gpu, WHITE);
	
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, gpu->scr_buf);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void
free_gpu(gb_gpu *gpu)
{
	free(gpu->scr_buf);

	/* Delete OpenGL buffers */
	glDeleteVertexArrays(1, &gpu->vao);
	glDeleteBuffers(1, &gpu->vbo);
	glDeleteTextures(1, &gpu->texture);
}

void
flip_screen(gb_gpu *gpu)
{
	BYTE *cpy, *top_row, *bot_row;

	cpy = malloc(gpu->scr_w * 3);
	for (int i = 0; i != gpu->scr_h / 2; ++i) {
		top_row = &gpu->scr_buf[((gpu->scr_h - 1) * (gpu->scr_w * 3)) - gpu->scr_w * 3 * i];
		bot_row = &gpu->scr_buf[gpu->scr_w * 3 * i];

		memcpy(cpy, bot_row, gpu->scr_w * 3);
		memcpy(bot_row, top_row, gpu->scr_w * 3);
		memcpy(top_row, cpy, gpu->scr_w * 3);
	}

	free(cpy);
}

void
clear_screen(gb_gpu *gpu, int color)
{
	for (int i = 0; i != gpu->scr_w * gpu->scr_h; ++i)
		memcpy(gpu->scr_buf + i * 3, &colors[color], 3);
}

void
draw_tile_row(const BYTE *data, BYTE *screen)
{
	BYTE data1, data2, color, tmp1, tmp2;

	data1 = *data;
	data2 = *(data + 1);

	for (int i = 0; i != 8; ++i) {
		tmp1 = (data1 >> (7 - i)) & 1;
		tmp2 = ((data2 >> (7 - i)) << 1) & 0x2;
		color = (tmp2 | tmp1) & 0x3;

		memcpy(screen + (i * 3), &colors[color], 3);
	}
}

void
draw_tile_at(gb_gpu *gpu, const BYTE *data, BYTE *screen)
{
	for (int i = 0; i != 8; ++i)
		draw_tile_row(data + (i * 2), screen + (i * gpu->scr_w * 3));
}

void
draw_tiles(gb_gpu *gpu)
{
	for (int i = 0; i != 0x100; ++i)
		draw_tile_at(gpu, &gpu->vram[i * 16],
			     &gpu->scr_buf[(i % 20) * 8 * 3 + ((i / 20) * 160 * 8 * 3)]);
}

void
render(gb_gpu *gpu)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(gpu->shader_program);
	
	glBindTexture(GL_TEXTURE_2D, gpu->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		     gpu->scr_w, gpu->scr_h, 0, GL_RGB, GL_UNSIGNED_BYTE, gpu->scr_buf);
	glBindVertexArray(gpu->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUseProgram(0);

	clear_screen(gpu, WHITE);
}

