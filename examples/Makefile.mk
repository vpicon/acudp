LDFLAGS = -L'../lib' -lacudp

SOURCES := $(wildcard ./*.c)
EXECS := $(patsubst %.c, %.exe, $(SOURCES))

all: $(EXECS) example_plus.exe

$(EXECS): %.exe: %.c
	$(CC) $(CFLAGS) -I'../include'  $< -o $@ $(LDFLAGS)

example_plus.exe: example_plus.cpp
	g++ $(CFLAGS) -I'../include' $< -o $@ $(LDFLAGS) 

.PHONY:	clean
clean:
	-rm $(EXECS) example_plus.exe

