cc=gcc
CFLAGS=-I.
DEPS=numeric.h
OBJS=numeric.o

%.o: %.cpp $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

add: $(OBJS) add.o
	$(CC) -o add numeric.o add.o  -lstdc++

convert: $(OBJS)  convert.o
	$(CC) -o convert numeric.o convert.o


clean:
	rm *.o convert add 	