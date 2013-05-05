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
	
	/* Set up freetype */
	if((error = FT_Init_FreeType(&library)) != 0)
		return error;

	/* Open up our font face */
	if((error = FT_New_Face(library, FONTPATH, 0, &face)) != 0)
		return error;
	FT_Set_Char_Size(face, 0, 8*64, 300, 300);
	
}

/* Draw the desired text in the current font face at the current raster position */
void text_print(char *t)
{
	int	n, error;

	for(n = 0; t[n] != '\0'; n++)
	{
		error = FT_Load_Char(face, t[n], FT_LOAD_RENDER);

		glBitmap(face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, 0, face->glyph->advance.x >> 6, 0, face->glyph->bitmap.buffer);
	}
}
