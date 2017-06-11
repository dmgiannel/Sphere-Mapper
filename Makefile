ifndef OS
	OS=$(shell uname)
endif
ifeq ($(OS), Linux)
	OPT += -Wall -g
	LIBS += -lm
endif

ifeq ($(OS), Windows_NT)
	CFLAGS +=  -Wl,--stack,16777216
endif

ifeq ($(OS), Darwin)
	OPT += -I/usr/local/include -Wall -Wno-deprecated -g
	PREDEF += -D_APPLE
	LIBS += -L/usr/local/lib
endif

ifeq ($(OS), Darwin)
	LIBS += -framework OpenGL -framework GLUT
endif
ifeq ($(OS), Windows_NT)
	LIBS += -lglew32 -lopengl32 -lglu32 -lfreeglut
else
	LIBS += -lGL -lGLU -lglut
endif

CC = gcc
LIBS+= -lpng -lz
VPATH = src: ../headers
IDIR = src/headers
SDIR = src/
DEPS = tools.h zpr.h
OBJ = tools.o viewer.o zpr.o
CFLAGS += -I$(IDIR) -std=c99 -g -Wno-deprecated-declarations

all :
	$(CC) $(OPT) $(SDIR)tools.c -c $(CFLAGS)
	$(CC) $(OPT) $(SDIR)viewer.c -c $(CFLAGS)
	$(CC) $(OPT) $(SDIR)zpr.c -c $(CFLAGS)
	$(CC) $(OPT) *.o -o viewer $(LIBS)
	rm -f *.o

	
.PHONY : clean

clean:
	rm -f *.o