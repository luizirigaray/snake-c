CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDLIBS = -lncurses

OBJS = main.o snake.o

snake: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c snake.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f snake *.o

.PHONY: clean