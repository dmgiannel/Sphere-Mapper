CC = gcc -std=c99
LIBS = -lGL -lGLU -lglut -lpng
VPATH = src: ../headers
IDIR = src/headers
DEPS = tools.h zpr.h
OBJ = tools.o viewer.o zpr.o
CFLAGS = -I$(IDIR) -std=c99 -g -Wl,--stack,16777216
	
%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)
		
viewer : $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
	rm -f *.o
	
.PHONY : clean

clean:
	rm -f *.o