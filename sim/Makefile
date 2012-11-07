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
	FLAGS   += `sdl-config --cflags`
	LDFLAGS +=  -lSDL -lSDL_image
endif

ifeq ($(UNAME), MINGW32_NT-6.1)
	FLAGS   += -I/MinGW/msys/1.0/include/ 
	LDFLAGS += -L/MinGW/msys/1.0/lib -lmingw32 -lSDLmain -lSDL -lSDL_image
	BINEXT = .exe
endif

FLAGS += -Wstrict-prototypes -Wundef -Wall -Wextra -Wunreachable-code  
FLAGS += -fsingle-precision-constant -funsigned-char    

SOURCES=$(wildcard ../firmware/animations/*.c)
BINARIES=$(SOURCES:../firmware/animations/%.c=%$(BINEXT))

LIB_SOURCES=$(wildcard ../firmware/libs/*.c)
LIB_HEADERS=$(wildcard ../firmware/libs/*.g)

all: $(BINARIES) _all_animations

clean:
	rm -f ${BINARIES} _all_animations

%$(BINEXT): ../firmware/animations/%.c $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	@echo "  \033[1;34mCompile\033[0m $< -> \033[1;33m $@\033[0m "
	@$(COMPILER) $(FLAGS) main.c $(LIB_SOURCES) $(LDFLAGS) $< -o $@ -I. 

_all_animations: $(SOURCES) $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	@echo "  \033[1;34mCompile all animations \033[0m -> \033[1;33m _all_animations \033[0m"
	@$(COMPILER) -DFULL=1 $(FLAGS) $(LIB_SOURCES) $(SOURCES) $(LDFLAGS) main.c -o _all_animations -I. 

.PHONY : clean all 

