# Makefile for wrapping cmake commands

EXEC=build/debug/bin/Sigma
CMAKE_FLAGS = #empty on purpose. set from command line like 'make rebuild CMAKE_FLAGS="cmakecmd=value"

all: $(EXEC)
.PHONY: all

$(EXEC): src CMakeLists.txt
	mkdir -p build/debug/bin
	mkdir -p build/bin
	cd build && cmake .. $(CMAKE_FLAGS) && make
	cp test.sc build/debug/bin
	cp test.sc build/bin

clean:
	@if [ -d build ] ; \
	then \
		rm -r build; \
	fi;	
.PHONY: clean

debug:
	make rebuild CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=Debug"
.PHONY: debug

rebuild:
	make clean
	make all
.PHONY: rebuild

eclipse: CMakeLists.txt
	make clean
	mkdir build
	cd build && cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
.PHONY: eclipse

codeblocks: CMakeLists.txt
	make clean
	mkdir build
	cd build && cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
.PHONY: codeblocks
