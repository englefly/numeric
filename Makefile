cc=gcc
CFLAGS=-I.
DEPS=numeric.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
numeric: numeric.o
	$(CC) -o numeric numeric.o

clean:
	rm *.o numeric