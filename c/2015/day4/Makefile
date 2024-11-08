CC = clang
CFLAGS = -Wall -Wextra

all: part1 part2

part1: src/part1.o
	$(CC) $(CFLAGS) -L../lib -o part1 $^ -laoc -lcrypto

part2: src/part2.o
	$(CC) $(CFLAGS) -L../lib -o part2 $^ -laoc -lcrypto

src/%.o: src/%.c
	$(CC) $(CFLAGS) -I../include -c -o $@ $^

.PHONY: clean
clean:
	-rm -rf src/*.o
