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
else
	LIBS += -lGL -lGLU -lglut
endif

CC = gcc
LIBS+= -lpng
VPATH = src: ../headers
IDIR = src/headers
DEPS = tools.h zpr.h
OBJ = tools.o viewer.o zpr.o
CFLAGS += -I$(IDIR) -std=c99 -g

%.o: %.c $(DEPS)
		$(CC) $(OPT) $(PREDEFS) -c -o $@ $< 
		
all : $(OBJ)
	$(CC) *.o -o viewer $(LIBS) $(CFLAGS)
	rm -f *.o

	
.PHONY : clean

clean:
	rm -f *.o