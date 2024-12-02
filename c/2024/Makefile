CC = clang
CFLAGS = -Wall -Wextra

lib: lib/libaoc.a

lib/aoc.o: lib/aoc.c
	$(CC) $(CFLAGS) -I./include -glldb -c -o lib/aoc.o lib/aoc.c

lib/libaoc.a: lib/aoc.o
	ar rcs lib/libaoc.a lib/aoc.o

.PHONY: clean
clean:
	-rm -rf lib/*.o lib/*.a
