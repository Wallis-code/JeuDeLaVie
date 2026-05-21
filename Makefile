CC     = gcc
CFLAGS = -Wall -std=c99 -I src $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
LIBS   = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)
BUILD  = build

SRCS = src/main.c src/core/grid.c src/core/history.c src/core/coord.c \
       src/UI/render.c src/UI/input.c src/UI/ui.c src/UI/pattern.c

SRCS_WATOR = src/main_wator.c src/core/wator/wator.c src/core/coord.c src/core/grid.c \
             src/UI/render.c src/UI/input.c src/UI/ui.c

OBJS       = $(patsubst src/%.c, $(BUILD)/%.o, $(SRCS))
OBJS_WATOR = $(patsubst src/%.c, $(BUILD)/%.o, $(SRCS_WATOR))

UI: $(OBJS)
	$(CC) $(OBJS) -o executable/JDLV $(LIBS)

WATOR: $(OBJS_WATOR)
	$(CC) $(OBJS_WATOR) -o executable/JDLV_wator $(LIBS)

$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

core: src/core/grid.c src/core/main.c
	$(CC) -Wall -std=c99 -I src -o executable/JDLV_terminal \
	src/core/grid.c src/core/main.c

test_grid: src/core/grid.c tests/test_grid.c
	$(CC) -Wall -std=c99 -I src -o tests/test_grid \
	src/core/grid.c tests/test_grid.c

analyse: src/core/grid.c src/core/analyse.c src/core/analyse_main.c
	$(CC) -Wall -std=c99 -I src -o executable/JDLV_analyse \
	src/core/grid.c src/core/analyse.c src/core/analyse_main.c

clean:
	rm -rf $(BUILD)
	rm -f executable/JDLV executable/JDLV_wator executable/JDLV_terminal \
	      tests/test_grid executable/JDLV_analyse