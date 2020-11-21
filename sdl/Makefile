# Compiler Settings
CC = gcc
CFLAGS = -g -Wall
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

# Files
INFILES=

.PHONY:	all clean

all: $(INFILES)
	$(CC) $(CFLAGS) $< -o $(basename $<) $(LFLAGS)
