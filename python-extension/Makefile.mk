SOURCES := $(wildcard src/*)

BUILD_DIR := build
TARGET  := acudp.cpython-38-x86_64-linux-gnu.so


# Rules
all: $(TARGET)

$(TARGET): setup.py $(SOURCES)
	python3 setup.py $(BUILD_DIR)

.PHONY: clean
clean: 
	-rm -r $(BUILD_DIR)
