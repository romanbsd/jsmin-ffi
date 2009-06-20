CC=g++
CXXFLAGS=-fPIC -O3

libjsmin.so.0:	jsmin.o
	$(CC) -shared -Wl,-soname,libjsmin.so.0 -o libjsmin.so.0 jsmin.o

jsmin.o:	jsmin.cpp jsmin.h

clean:
	rm -f *.o *.so.0

install:
	cp libjsmin.so.0 ../lib
