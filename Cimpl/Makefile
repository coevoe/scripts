CC		= gcc
CFLAGS 	= -Wall -g

default: all

objects = dwm-bat dwm-date dwm-temp dwm-krnl
all: $(objects)

$(objects): %: src/%.c
		$(CC) $(CFLAGS) $< -o $@
