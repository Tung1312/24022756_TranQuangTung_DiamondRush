# Makefile for Diamond Rush

CC = g++
CFLAGS = -Wall -g $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf)
LIBS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf)

# Output files
LINUX_OUT = DiamondRush
WINDOWS_OUT = DiamondRush.exe

# Source files
SOURCES = main.cpp

# Targets
all: linux

linux: $(SOURCES)
	$(CC) $(CFLAGS) -o $(LINUX_OUT) $(SOURCES) $(LIBS)

windows: $(SOURCES)
	$(CC) $(CFLAGS) -o $(WINDOWS_OUT) $(SOURCES) $(LIBS)

run: linux
	./$(LINUX_OUT)

clean:
	rm -f $(LINUX_OUT) $(WINDOWS_OUT)

.PHONY: all linux windows run clean