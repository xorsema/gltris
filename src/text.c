#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include "gltris.h"
#include "graphics.h"
#include "text.h"

FT_Library library;
FT_Face face;

int text_init(void)
{
	int error;
	
	error = FT_Init_FreeType(&library);
	if(error != 0)
		return error;

	error = FT_New_Face(library, FONTPATH, 0, &face);
	if(error != 0)
		return error;

	error = FT_Set_Char_Size(face, 0, 16*64, 96, 96);
	if(error != 0)
		return error;

	return 0;
}

/* Expand the current bitmap to fit the GL_LUMINANCE_ALPHA format, essentially doubling the data */
static void *expand_texture(void)
{
	int	 x, y, w, h;
	GLubyte *nd; 
	GLubyte *od; 
	
	nd = (GLubyte*)malloc(sizeof(GLubyte) * 2 * (face->glyph->bitmap.width) * (face->glyph->bitmap.rows));
	memset(nd, 0, sizeof(GLubyte));
	od = face->glyph->bitmap.buffer;
	w  = face->glyph->bitmap.width;
	h  = face->glyph->bitmap.rows;

	for(y = 0; y < h; y++)
	{
		for(x = 0; x < w; x++)
		{
			nd[2*(y*w + x)] = nd[2*(y*w + x) + 1] = od[(y*w) + x];
		}
	}

	return nd;
}

static void text_gen_texture(char c, GLuint *out)
{
	GLuint	 result;
	GLubyte *buf;

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	buf = expand_texture();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buf);

	*out = result;

	free(buf);
}

void text_print(float x, float y, char *t)
{
	GLuint	texname;
	int	n;
	float	pen_x = 0;
	float	pen_y = 0;

	for(n = 0; t[n] != 0; n++)
	{
		
		if(FT_Load_Char(face, t[n], FT_LOAD_RENDER) != 0)
			continue;

		text_gen_texture(t[n], &texname);
		render_textured_quad(texname, x+pen_x, y+pen_y, (face->glyph->bitmap).width, (face->glyph->bitmap).rows);

		pen_x += face->glyph->advance.x >> 6;
		pen_y += face->glyph->advance.y >> 6;
	}
}

void get_text_size(const char *t, uint32_t *ow, uint32_t *oh)
{
	int		n;
	uint32_t	w = 0;
	uint32_t	h = 0;

	for(n = 0; t[n] != 0; n++)
	{
		if(FT_Load_Char(face, t[n], FT_LOAD_RENDER) != 0)
			continue;
		h = (h == 0) ? (face->glyph->bitmap_top) : h;
		h = (face->glyph->bitmap_top) > h ? (face->glyph->bitmap_top) : h;
		w += (face->glyph->bitmap_left) + (face->glyph->advance.x >> 6);
	} 

	*ow = w;
	*oh = h;
}
