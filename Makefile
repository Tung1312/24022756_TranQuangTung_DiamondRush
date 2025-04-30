# Makefile for Diamond Rush

# Compiler settings
CC = g++
CFLAGS = -Wall -g -Iinclude $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf)
LIBS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf)

# Source files
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Output files
LINUX_OUT = DiamondRush
WINDOWS_OUT = DiamondRush.exe

# Targets
all: linux

linux: $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(LINUX_OUT) $(SRC_FILES) $(LIBS)

windows: $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(WINDOWS_OUT) $(SRC_FILES) $(LIBS)

run: linux
	./$(LINUX_OUT)

debug: linux
	gdb ./$(LINUX_OUT)

clean:
	rm -f $(LINUX_OUT) $(WINDOWS_OUT)

.PHONY: all linux windows run debug clean