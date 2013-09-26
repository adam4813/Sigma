Sigma
===

The Sigma game engine for creating games on multiple platforms. The name Sigma was chosen due to its meaning in math of summation notation. In Sigma, engine components are assigned to entities, and it is the sum of all the components that makes an entity.

Requires CMake and a compiler that supports C++11.


Installing
===

GUI Mode
* Run CMake and set the source directory to the root folder.
* Set the build directory to root/build/
* Configure and generate your make/project/solutions files.
* Build and run.

Command Mode
* mkdir build/
* cd build/
* cmake ..
* make

Finally you must copy the shaders folder and test.sc into the executable folder. E.g. build/bin/ or maybe build/bin/Debug|Release.

Using the makefile-wrapper
* make rebuild # make clean; make

Eclipse CDT (Unix/Linux)
* make eclipse

Running
===

__(Unix/Linux)__

The binary `Sigma` is written to `build/bin`, and can be run with:

	./build/bin/Sigma

Visual Studio
===
You must change the startup project via right-clicking on the project in the solution explorer in order for Debugging to launch the correct program.
Also you must go into the project's properties, and under the 'Debugging' category change the 'Working Directory' to '$(OutDir)'.
