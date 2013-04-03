CC = gcc
CFLAGS = -std=c99 -g
LINK = -lGL -lGLU -lSDL -lSDL_ttf -lm
VPATH = src/ : obj/
OUT = bin/
OBJ = obj/
SRCS = gltris.c graphics.c input.c block.c timer.c

.PHONY: all clean
all: gltris

include $(SRCS:.c=.d)

gltris : $(SRCS:.c=.o)
	$(CC) $(CFLAGS) $(LINK) -o $(OUT)$@ $^ 

%.o : %.c
	$(CC) $(CFLAGS) $(LINK) -c -o $@ $<

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean :
	rm -f ./*.o; \
	rm -f ./bin/gltris
