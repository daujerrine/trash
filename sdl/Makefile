# Compiler Settings
CC = gcc
CFLAGS = -g -Wall
LFLAGS = -lSDL2 -lSDL2_image

# Files
INFILES=

.PHONY:	all clean

all: $(INFILES)
	$(CC) $(CFLAGS) $< -o $(basename $<) $(LFLAGS)
