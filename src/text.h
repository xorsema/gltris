#ifndef TEXT_H
#define TEXT_H

//#define FONTPATH "res/Dotrice-Regular.otf"
//#define FONTPATH "/usr/share/wine/fonts/tahoma.ttf"
#define FONTPATH "res/Segment14.otf"

int text_init(void);
void text_print(float, float, unsigned int, char*);
void get_text_size(const char*, unsigned int, uint32_t*, uint32_t*);
void print_game_info(void);
void print_gameover_text(void);

#endif
