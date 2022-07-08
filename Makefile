CC = clang
CFLAGS = -std=c99 -Wall -Werror -pedantic -g

.PHONY: all clean

all: main

main: main.o arraylist.o hashmap.o yacjson-core.o

main.o:

arraylist.o: arraylist.h

hashmap.o: hashmap.h

yacjson-core.o: yacjson-core.h

yacxml-core.o: yacxml-core.h

clean:
	rm -f ./main
	rm -f ./*.o
