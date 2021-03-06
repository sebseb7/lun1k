UNAME := $(shell uname)

COMPILER = gcc


FLAGS= -I. -I.. -Ihal/inc --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare -Wunreachable-code  -Wstrict-prototypes  #-fsingle-precision-constant
LDFLAGS=
BINEXT=

ifeq ($(UNAME), Darwin)
	COMPILER = gcc
	FLAGS   += -Qunused-arguments
	
	FLAGS   += `sdl-config --cflags` 
	LDFLAGS += `sdl-config --libs` -lSDL_image 
endif

ifeq ($(UNAME), Linux)
	FLAGS   += -DNOMALLOCWRAP=1 `sdl-config --cflags`
	LDFLAGS += `sdl-config --libs` -lSDL_image 
endif

ifeq ($(UNAME), MINGW32_NT-6.1)
	FLAGS   += -I/MinGW/msys/1.0/include/ 
	LDFLAGS += -L/MinGW/msys/1.0/lib -lmingw32 -lSDLmain -lSDL -lSDL_image 
	BINEXT = .exe
endif

SOURCES=$(wildcard ../animations/*.c)
BINARIES=$(SOURCES:../animations/%.c=%$(BINEXT))

LIB_SOURCES=$(wildcard ../libs/*.c ../libs/mcugui/*.c ../animations/bd/*.c ../animations/lib/*.c ../libs/zint/*.c hal/src/*.c)
LIB_HEADERS=$(wildcard ../libs/*.h ../libs/mcugui/*.c ../animations/bd/*.h ../animations/lib/*.h)

all: $(BINARIES) _all_animations

clean:
	rm -f ${BINARIES} _all_animations

%$(BINEXT): ../animations/%.c $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	@echo "  \033[1;34mCompile\033[0m $< -> \033[1;33m $@\033[0m "
	@$(COMPILER) $(FLAGS) main.c $(LIB_SOURCES) $(LDFLAGS) -lm $< -o $@ -I. 

_all_animations: $(SOURCES) $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	@echo "  \033[1;34mCompile all animations \033[0m -> \033[1;33m _all_animations \033[0m"
	@$(COMPILER) -DFULL=1 $(FLAGS) main.c $(LIB_SOURCES) $(SOURCES) $(LDFLAGS) -lm  -o _all_animations -I. 

.PHONY : clean all 

