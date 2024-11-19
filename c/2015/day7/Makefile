CC = clang
CFLAGS = -Wall -Wextra

all: part1 part2

part1: src/part1.o src/lexer.o src/parser.o src/interpreter.o
	$(CC) $(CFLAGS) -L../lib -o part1 $^ -laoc

part2: src/part2.o src/lexer.o src/parser.o src/interpreter.o
	$(CC) $(CFLAGS) -L../lib -o part2 $^ -laoc

src/%.o: src/%.c
	$(CC) $(CFLAGS) -glldb -I../include -c -o $@ $^

.PHONY: clean
clean:
	-rm -rf src/*.o
