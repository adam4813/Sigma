Sigma
===

The Sigma game engine for creating games on multiple platforms. The name of Sigma was chosen due to its meaning in math of summation notation. In Sigma engine components are assigned to entities, and it is the sum of all the components that makes an entity.

Requires CMake and a compiler that supports C++11.


Installing
===

* Run CMake and set the srouce directory to the root folder.
* Set the build directory to root/build/
* Configure and generate your make/project/solutions files.
* Build and run.

Visual Studio
===
You must change the startup project via right-clicking on the project in the solution explorer, in order for Debugging to luanch the correct program.
Also you must go into the project's properties, and under the 'Debugging' category change the 'Working Directory' to '$(OutDir)'.
