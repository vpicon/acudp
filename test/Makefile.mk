LDFLAGS = -L'../lib' -lacudp -lcmocka

SOURCES := $(wildcard unit/*.c)
EXECS := $(patsubst %.c, %.exe, $(SOURCES))

all: $(EXECS) 

$(EXECS): %.exe: %.c
	$(CC) $(CFLAGS) -I'../include'  $< -o $@ $(LDFLAGS)


.PHONY:	clean
clean:
	-rm $(EXECS) 
