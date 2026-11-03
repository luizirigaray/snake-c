CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDLIBS = -lncurses

OBJS = main.o snake.o

snake: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c snake.h
	$(CC) $(CFLAGS) -c $<

tests: tests.o snake.o
	$(CC) $(CFLAGS) -o $@ tests.o snake.o

test: tests
	./tests

clean:
	rm -f snake tests *.o

.PHONY: clean test