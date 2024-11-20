CC = clang
CFLAGS = -Wall -Wextra

all: part1 part2

part1: src/part1.o
	$(CC) $(CFLAGS) -L../lib -o part1 $^ -laoc

part2: src/part2.o
	$(CC) $(CFLAGS) -L../lib -o part2 $^ -laoc

src/%.o: src/%.c
	$(CC) $(CFLAGS) -I../include -g -c -o $@ $^

.PHONY: clean
clean:
	-rm -rf src/*.o
