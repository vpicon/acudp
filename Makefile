CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wshadow -pedantic -O3
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
.PHONY: lib bindirs examples test clean
all: bindirs $(LIBRARY) examples test

bindirs:
	@if [ ! -d 'bin' ]; then mkdir 'bin'; fi
	@if [ ! -d 'lib' ]; then mkdir 'lib'; fi

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

bin/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@ -c

examples: 
	@$(MAKE) --no-print-directory --directory='examples' -f Makefile.mk

test: 
	@$(MAKE) --no-print-directory --directory='test' -f Makefile.mk

clean:
	-rm $(OBJECTS) $(LIBRARY)
	@$(MAKE) --no-print-directory --directory='examples' -f Makefile.mk clean
