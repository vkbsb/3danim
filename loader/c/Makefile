CC = gcc
CFLAGS = -Wall -Werror -g3
LDFLAGS = -Wall -g3 -lglut -lGLU -lGL

output:sample.o anim3d.o
	$(CC) $(LDFLAGS) -o output sample.o anim3d.o
sample.o:sample.c
	$(CC) $(CFLAGS) -c sample.c
anim3d.o:anim3d.c
	$(CC) $(CFLAGS) -c anim3d.c

.PHONEY clean:
	rm -vf *.o output *.pyc
