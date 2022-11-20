CC = gcc

FILES = metamon.c

COMPILE_FLAGS = -Wall -g -pipe -O2 -march=native -std=c11

LIBRARY_OBJECT_FILE = metamonlib.o

SHARED_LIBRARY_FILE = libmetamon.so

LIB_FLAGS = -lm -fPIC

OWNER = aca

main: $(FILES)
	$(CC) -fPIC $(COMPILE_FLAGS) -o $(LIBRARY_OBJECT_FILE) -c $(FILES)

lib: $(LIBRARY_OBJECT_FILE)
	$(CC) -shared $(COMPILE_FLAGS) -o $(SHARED_LIBRARY_FILE) $(LIBRARY_OBJECT_FILE) $(LIB_FLAGS)

install:
	install -o aca metamon.h /usr/include/
	install -o aca libmetamon.so /usr/lib/
all:
	make 
	make lib
	make install
