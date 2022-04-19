SOURCES := $(wildcard ./*.c)
OBJECTS := $(patsubst src/%.c, bin/examples/%.o, $(SOURCES))
EXECS := $(patsubst %.c, %, $(SOURCES))

all: $(EXECS)

$(EXECS): %: %.c
	$(CC) $(CFLAGS) -I'../include'  $< -o $@ -L'../lib' -lacudp
