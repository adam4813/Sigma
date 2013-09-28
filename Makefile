# Makefile for wrapping cmake commands

EXEC=build/debug/bin/Sigma

all: $(EXEC)
.PHONY: all

$(EXEC): src CMakeLists.txt
	mkdir -p build/debug/bin
	mkdir -p build/bin
	cd build && cmake .. && make
	cp test.sc build/debug/bin
	cp test.sc build/bin

clean:
	@if [ -d build ] ; \
	then \
		rm -r build; \
	fi;	
.PHONY: clean

rebuild:
	make clean
	make all
.PHONY: rebuild

eclipse: CMakeLists.txt
	make clean
	mkdir build
	cd build && cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
.PHONY: eclipse
