CC=gcc
CFLAGS=-I.
DEPS=numeric.h
OBJS=numeric.o

%.o: %.cpp $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

# performance: $(OBJS) performance.o
# 	$(CC) -o performance.exe performance.o numeric.o -lstdc++
divid:$(OBJS) divid.o
	$(CC) -o divid.exe divid.o numeric.o -lstdc++

multi:$(OBJS) multi.o
	$(CC) -o multi.exe numeric.o multi.o  -lstdc++

sub: $(OBJS) sub.o
	$(CC) -o sub.exe numeric.o sub.o  -lstdc++

add: $(OBJS) add.o
	$(CC) -o add.exe numeric.o add.o  -lstdc++

convert: $(OBJS)  convert.o
	$(CC) -o convert.exe numeric.o convert.o


clean:
	rm *.o *.exe 