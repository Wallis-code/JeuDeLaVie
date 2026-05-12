CC     = gcc
CFLAGS = -Wall -std=c99 -I src $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
LIBS   = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)

core: src/core/grid.c src/core/main.c
	$(CC) -Wall -std=c99 -I src -o executable/JDLV_terminal src/core/grid.c src/core/main.c

test_grid: src/core/grid.c tests/test_grid.c
	$(CC) -Wall -std=c99 -I src -o tests/test_grid src/core/grid.c tests/test_grid.c

UI: src/main.c src/core/grid.c src/core/history.c src/UI/render.c src/UI/input.c src/UI/ui.c
	$(CC) $(CFLAGS) -o executable/JDLV src/main.c src/core/grid.c src/core/history.c src/UI/render.c src/UI/input.c src/UI/ui.c $(LIBS)
clean:
	rm -f tests/test_grid
	rm -f executable/JDLV_terminal
	rm -f executable/JDLV