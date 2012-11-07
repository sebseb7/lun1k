UNAME := $(shell uname)

COMPILER = gcc


FLAGS=--std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare -Wunreachable-code -pedantic  -Wstrict-prototypes
LDFLAGS=
BINEXT=

ifeq ($(UNAME), Darwin)
	COMPILER = clang
	FLAGS   += `sdl-config --cflags`
	LDFLAGS += `sdl-config --libs` -lSDL_image -lSDL_ttf
endif

ifeq ($(UNAME), Linux)
	FLAGS   += `sdl-config --cflags`
	LDFLAGS +=  -lSDL -lSDL_image -lSDL_ttf
endif

ifeq ($(UNAME), MINGW32_NT-6.1)
	FLAGS   += -I/MinGW/msys/1.0/include/ 
	LDFLAGS += -L/MinGW/msys/1.0/lib -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf
	BINEXT = .exe
endif

SOURCES=$(wildcard ../firmware/animations/*.c)
BINARIES=$(SOURCES:../firmware/animations/%.c=%$(BINEXT))

LIB_SOURCES=$(wildcard ../firmware/libs/*.c)
LIB_HEADERS=$(wildcard ../firmware/libs/*.g)

all: $(BINARIES) _full

clean:
	rm -f ${BINARIES}

%$(BINEXT): ../firmware/animations/%.c $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	$(COMPILER) $(FLAGS) main.c $(LIB_SOURCES) $(LDFLAGS) $< -o $@ -I. 

_full: $(SOURCES) $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	$(COMPILER) -DFULL=1 $(FLAGS) $(LIB_SOURCES) $(SOURCES) $(LDFLAGS) main.c -o _full -I. 

