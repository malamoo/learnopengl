CFLAGS = -Wall -Werror -std=c99
LIBS = -lm -ldl -pthread

GLFW_CFLAGS = -w
GLFW_DEFS = -DLSH_GLFW_IMPLEMENTATION 
GLAD_CFLAGS = -w
STB_CFLAGS = -w

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	GLFW_DEFS += -D_GLFW_X11 -DLSH_GLFW_X11
endif
ifeq ($(UNAME_S), Darwin)
	GLFW_DEFS += -D_GLFW_COCOA -DLSH_GLFW_COCOA
	GLFW_CFLAGS += -ObjC
	GLFW_FWS = -framework Cocoa -framework IOKit
endif

.PHONY: all
all: mkdir bin/game

.PHONY: mkdir
mkdir:
	mkdir -p build bin

bin/game: build/glfw.o build/glad.o build/stb_image.o build/shader.o \
	  build/game.o
	cc $(GLFW_FWS) -o $@ $^ $(LIBS)

build/glfw.o: ext/glfw.c
	cc $(GLFW_CFLAGS) $(GLFW_DEFS) $(GLFW_FWS) -c $^ -o $@

build/glad.o: ext/glad.c
	cc $(GLAD_CFLAGS) -c $^ -o $@

build/stb_image.o: ext/stb_image.c
	cc $(STB_CFLAGS) -c $^ -o $@

build/shader.o: src/shader.c
	cc $(CFLAGS) -c $^ -o $@

build/game.o: src/game.c
	cc $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf build bin
