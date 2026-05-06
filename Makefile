CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I src

test_grid: src/core/grid.c tests/test_grid.c
	$(CC) $(CFLAGS) -o test_grid src/core/grid.c tests/test_grid.c

clean:
	rm -f test_grid