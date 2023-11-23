CC=gcc
CFLAGS=-Wall
LIBS=-lglfw -lGLEW -lGL -lm


main: main.c
	$(CC) $(CFLAGS) main.c -o main $(LIBS)

clean:
	rm -f main
