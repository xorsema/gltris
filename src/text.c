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

static void text_gen_texture(char c, GLuint *out)
{
	GLuint result;

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

	*out = result;
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
