CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I src

main: src/main.c
	$(CC) $(CFLAGS) -o JDLV_terminal src/core/grid.c src/main.c
test_grid: src/core/grid.c tests/test_grid.c
	$(CC) $(CFLAGS) -o tests/test_grid src/core/grid.c tests/test_grid.c

clean:
	rm -f tests/test_grid
	rm -f JDLV_terminal