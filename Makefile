# Makefile for wrapping cmake commands

EXEC=build/debug/bin/Sigma

all: $(EXEC)
.PHONY: all

$(EXEC): src CMakeLists.txt
	mkdir build
	cd build && cmake .. && make
	cp test.sc build/debug/bin
	cd build/debug/bin

clean:
	rm -r build
.PHONY: clean

rebuild:
	make clean
	make all
.PHONY: rebuild
