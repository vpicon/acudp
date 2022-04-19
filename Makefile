CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wshadow -pedantic 
CFLAGS += -I'include/'

AR = ar  # archiver
ARFLAGS = rcs

export CC CFLAGS


# Main target
LIBRARY := lib/libacudp.a


# Track files
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)
OBJECTS := $(patsubst src/%.c, bin/%.o, $(SOURCES))


# Rules
.PHONY: lib bindirs examples
all: bindirs $(LIBRARY) examples 

bindirs:
	@if [ ! -d 'bin' ]; then mkdir 'bin'; fi
	@if [ ! -d 'lib' ]; then mkdir 'lib'; fi

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

bin/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@ -c


examples: 
	@$(MAKE) --no-print-directory --directory='examples' -f Makefile.mk
