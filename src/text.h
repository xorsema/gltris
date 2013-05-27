#ifndef TEXT_H
#define TEXT_H

//#define FONTPATH "res/Dotrice-Regular.otf"
//#define FONTPATH "/usr/share/wine/fonts/tahoma.ttf"
#define FONTPATH "res/Segment14.otf"

int text_init(void);
void text_print(float, float, char*);
void get_text_size(const char*, uint32_t*, uint32_t*);
#endif
