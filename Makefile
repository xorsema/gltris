CC = gcc
SDL_CFLAGS = $(shell sdl-config --cflags)
FT_CFLAGS = $(shell freetype-config --cflags)
SDL_LDFLAGS = $(shell sdl-config --libs)
FT_LDFLAGS = $(shell freetype-config --libs)
CFLAGS = -std=c99 -g $(SDL_CFLAGS) $(FT_CFLAGS)
LINK = -lGL -lGLU -lm  $(SDL_LDFLAGS) $(FT_LDFLAGS)
VPATH = src/ : obj/
OUT = bin/
OBJ = obj/
SRCS = gltris.c graphics.c input.c game.c block.c timer.c text.c

.PHONY: all clean
all: gltris

include $(SRCS:.c=.d)

gltris : $(SRCS:.c=.o)
	$(CC) $(CFLAGS) $(LINK) -o $(OUT)$@ $^ 

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean :
	rm -f ./*.o; \
	rm -f ./bin/gltris
