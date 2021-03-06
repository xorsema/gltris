#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <ft2build.h>
#include <freetype2/freetype.h>
#include <freetype2/ftglyph.h>

#include "gltris.h"
#include "graphics.h"
#include "text.h"
#include "block.h"
#include "game.h"

FT_Library library;
FT_Face face;

/* Make sure freetype is up and running */
int text_init(void){
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
static void *expand_texture(void){
  int	 x, y; 
  int	 w, h;
  GLubyte	*nd; 
  GLubyte	*od; 
	
  nd = (GLubyte*)malloc(sizeof(GLubyte) * 2 * (face->glyph->bitmap.width) * (face->glyph->bitmap.rows));
  memset(nd, 0, sizeof(GLubyte));
  od = face->glyph->bitmap.buffer;
  w  = face->glyph->bitmap.width;
  h  = face->glyph->bitmap.rows;

  for(y = 0; y < h; y++){
    for(x = 0; x < w; x++){
      nd[2*(y*w + x)] = nd[2*(y*w + x) + 1] = od[(y*w) + x];
    }
  }

  return nd;
}

/* Generate a texture from the current bitmap character in the slot */
static void text_gen_texture(GLuint *out){
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

/* Print the text to the screen, requires a proper OpenGL context */
void text_print(float x, float y, unsigned int size, char *t){
  GLuint	texname;
  int	n;
  float	pen_x = 0;
  float	pen_y = 0;

  FT_Set_Char_Size(face, 0, size*64, 96, 96);	

  for(n = 0; t[n] != 0; n++){
		
    if(FT_Load_Char(face, t[n], FT_LOAD_RENDER) != 0)
      continue;

    text_gen_texture(&texname);
    render_textured_quad(texname, x+pen_x, y+pen_y, (face->glyph->bitmap).width, (face->glyph->bitmap).rows);
    glDeleteTextures(1, &texname);

    pen_x += face->glyph->advance.x >> 6;
    pen_y += face->glyph->advance.y >> 6;
  }
}

/* Get the width and height of the specified string with the specified font size */
void get_text_size(const char *t, unsigned int size, uint32_t *ow, uint32_t *oh){
  int		n;
  uint32_t	w = 0;
  uint32_t	h = 0;

  FT_Set_Char_Size(face, 0, size*64, 96, 96);

  for(n = 0; t[n] != 0; n++){
    if(FT_Load_Char(face, t[n], FT_LOAD_RENDER) != 0)
      continue;
    h = (h == 0) ? (face->glyph->bitmap_top) : h;
    h = (face->glyph->bitmap_top) > h ? (face->glyph->bitmap_top) : h;
    w += (face->glyph->bitmap_left) + (face->glyph->advance.x >> 6);
  } 

  *ow = w;
  *oh = h;
}

/* Print some info about the current game */
void print_game_info(void){
  char buf[100];
  unsigned int w, h, t;

  sprintf(buf, "Level %i", g_game.level);
  get_text_size(buf, 18, &w, &h);
  glColor3f(1.0, 1.0, 1.0);
  text_print(WWIDTH-w, WHEIGHT-h, 18, buf);
  t = h;

  memset(buf, 0, sizeof(buf));
  sprintf(buf, "Rows %i", g_game.rows_cleared);
  get_text_size(buf, 18, &w, &h);
  text_print(WWIDTH-w, WHEIGHT-h-t-10, 18, buf);
  t += h;

  memset(buf, 0, sizeof(buf));
  sprintf(buf, "Score %i", g_game.score);
  get_text_size(buf, 18, &w, &h);
  text_print(WWIDTH-w, WHEIGHT-h-t-20, 18, buf);
}

void print_gameover_text(void){
  char buf[100];
  unsigned int w, h, t;

  sprintf(buf, SCORETXT, g_game.score);
  get_text_size(buf, 24, &w, &h);
  glColor3f(1.0, 1.0, 1.0);
  text_print((WWIDTH-w)/2, (WHEIGHT-h)/2, 24, buf);
  t = h;

  get_text_size(GAMEOVERTXT, 12, &w, &h);
  text_print((WWIDTH-w)/2, ((WHEIGHT-h-t)/2)-20, 12, GAMEOVERTXT);
}
