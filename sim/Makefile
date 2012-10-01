UNAME := $(shell uname)

COMPILER = gcc


FLAGS=--std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare -Wunreachable-code -pedantic  -Wstrict-prototypes
LDFLAGS=
BINEXT=

ifeq ($(UNAME), Darwin)
	COMPILER = clang
	FLAGS   += `sdl-config --cflags`
	LDFLAGS += `sdl-config --libs` -lSDL_image
endif

ifeq ($(UNAME), Linux)
	LDFLAGS +=  -lSDL -lSDL_image
endif

ifeq ($(UNAME), MINGW32_NT-6.1)
	FLAGS   += -I/MinGW/msys/1.0/include/ 
	LDFLAGS += -L/MinGW/msys/1.0/lib -lmingw32 -lSDLmain -lSDL -lSDL_image
	BINEXT = .exe
endif

SOURCES=$(wildcard ../firmware/animations/*.c)
BINARIES=$(SOURCES:../firmware/animations/%.c=%$(BINEXT))

LIB_SOURCES=$(wildcard ../firmware/libs/*.c)
LIB_HEADERS=$(wildcard ../firmware/libs/*.g)

all: $(BINARIES)

clean:
	rm -f ${BINARIES}

%$(BINEXT): ../firmware/animations/%.c $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	$(COMPILER) $(FLAGS) main.c $(LIB_SOURCES) $(LDFLAGS) $< -o $@ -I. 

