LDFLAGS = -L'../lib' -lacudp

SOURCES := $(wildcard ./*.c)
OBJECTS := $(patsubst src/%.c, bin/examples/%.o, $(SOURCES))
EXECS := $(patsubst %.c, %, $(SOURCES))

all: $(EXECS) example_plus

$(EXECS): %: %.c
	$(CC) $(CFLAGS) -I'../include'  $< -o $@ $(LDFLAGS)

example_plus: example_plus.cpp
	g++ -I'../include' $< -o $@ $(LDFLAGS) 