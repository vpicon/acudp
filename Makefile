CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wshadow -pedantic 
CFLAGS += -I'include/'

# Main target
LIBRARY := lib/libacudp.a

# Track files
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)
OBJECTS := $(patsubst src/%.c, bin/%.o, $(SOURCES))


# Rules
.PHONY: lib bindirs
all: bindirs $(LIBRARY) examples 

bindirs:
	@if [ ! -d 'bin' ]; then mkdir 'bin'; fi
	@if [ ! -d 'lib' ]; then mkdir 'lib'; fi


$(LIBRARY): $(OBJECTS)


bin/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@ -c



