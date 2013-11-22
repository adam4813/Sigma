Sigma
===

The Sigma game engine for creating games on multiple platforms. The name Sigma was chosen due to its meaning in math of summation notation. In Sigma, engine components are assigned to entities, and it is the sum of all the components that makes an entity.

Requires CMake and a compiler that supports C++11.


Contributing
===
The easiest way to contribute is to fork and experiement. To better understand the orgranization please refer to https://docs.google.com/drawings/d/1HFFiz1oJc3A8gyO-fMADAfW8PTWjCqhe5silYDXM1lo/edit. 

Coding standards and practices can be found here https://docs.google.com/document/d/1dN3cNUdn0HKb7lZXcBCIL2Tm216hQAQxbZnEuCpiYtc/edit

Installing
===

If you want to compile Sigma on Linux, you need to install dependencies separately. On Debian-based distributions, you can use the following command as root to obtain them:

    apt-get install libglew-dev libsoil-dev libglm-dev libsdl2-dev libbullet-dev libsdl2-ttf

Ubuntu will not contain all the required sdl2 dependencies until Trusting (14.04). The following ppa will need to be added:

    sudo add-apt-repository ppa:zoogie/sdl2-snapshots

The google test package will need to be compiled manually:

    sudo apt-get install libgtest-dev
    cd /usr/src/gtest
    sudo cmake CMakeLists.txt
    sudo make
    sudo cp *.a /usr/lib

Versions of ubuntu older than quantal (12.10) will need to compile bullet manually:

    sudo apt-get -y install automake libtool cmake freeglut3-dev subversion
    svn checkout http://bullet.googlecode.com/svn/trunk/ bullet-read-only
    cd bullet-read-only
    cmake . -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON
    make -j10 && sudo make install

GUI Mode
---

* Run CMake and set the source directory to the root folder.
* Set the build directory to root/build/
* Configure and generate your make/project/solutions files.
* Build and run.

Command Mode
---

```sh
mkdir build/
cd build/
cmake ..
make
```


Using the makefile-wrapper
---

```sh
make rebuild
make clean
make
```

Eclipse CDT (Unix/Linux)
---

```sh
make eclipse
```

After compiling
---

Finally you must copy the shaders folder and test.sc into the executable folder. E.g. build/bin/ or maybe build/bin/Debug|Release.

Running
===

__(Unix/Linux)__

The binary `Sigma` is written to `build/bin`, `build/bin/debug`, or `build/debug/bin`

Visual Studio
===
You must change the startup project via right-clicking on the project in the solution explorer in order for Debugging to launch the correct program.
Also you must go into the project's properties, and under the 'Debugging' category change the 'Working Directory' to '$(OutDir)'.
