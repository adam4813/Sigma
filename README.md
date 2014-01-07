Sigma
===

The Sigma game engine for creating games on multiple platforms. The name Sigma was chosen due to its meaning in math of summation notation. In Sigma, engine components are assigned to entities, and it is the sum of all the components that makes an entity.

Requires CMake and one of the following compilers:

* GCC 4.8 or newer;
* Clang 3.3 or newer;
* Visual Studio 2010 or newer;

Contributing
===
The easiest way to contribute is to fork and experiment. To better understand the organization please refer to [this drawing](https://docs.google.com/drawings/d/1HFFiz1oJc3A8gyO-fMADAfW8PTWjCqhe5silYDXM1lo/edit).

Coding standards and practices can be found [here](https://github.com/adam4813/Sigma/wiki/Coding-Standards).

Installing
===
Sigma requires the following dependencies:
* [GLEW](http://glew.sourceforge.net) 1.5.2 or newer, not required on OS X;
* [GLFW](http://www.glfw.org) 3.0.0 or newer;
* [GLM](http://glm.g-truc.net);
* [SOIL](http://www.lonesock.net/soil.html);
* [Bullet](http://www.bulletphysics.org);
* [Awesomium](http://www.awesomium.com);
* An OpenAL API implementation;
* [libogg](https://www.xiph.org/ogg/);
* [libvorbis](https://www.xiph.org/ogg/);

You'll also need a [package of assets](http://wiki.trillek.org/wiki/Assets).  Unpack it in the build/bin/ directory.

## Building ##

Use CMake to create makefiles or projects. The resulting executable will be saved in build/bin/.

## OS X ##

If you want to compile Sigma on OS X, your system must be running OS X 10.7 or later.  You will need to have installed Xcode and, unless your system is running 10.9, the Command Line Tools.  Both can be downloaded from the [Apple Developer Website](https://developer.apple.com/downloads).  

Before you can build Sigma, you'll first need to install the dependencies separately.  Using a package manager, such as [MacPorts](http://macports.org), is recommended.  Sigma can only be built as a 32-bit executable under OS X.  All dependencies, except cmake, that you install must include a 32-bit slice.

If MacPorts is present, you can use the following commands to install CMake, GLM, GLFW, libogg and libvorbis:

	sudo port install cmake
	sudo port install glm +universal
	sudo port install glfw +universal
	sudo port install libogg +universal
	sudo port install libvorbis +universal

The current Macports version of Bullet does not include all of the libraries Sigma depends on.  You will need to compile Bullet manually.

	svn checkout http://bullet.googlecode.com/svn/trunk/ bullet-read-only
	cd bullet-read-only
	cmake . -G "Unix Makefiles" -DCMAKE_OSX_ARCHITECTURES='i386;x86_64' -DBUILD_DEMOS=OFF
	make -j10 && sudo make install 

Sigma on OS X requires a [patched version](https://github.com/DeVaukz/SOIL) of SOIL that must be installed manually.  

	git clone https://github.com/DeVaukz/SOIL.git
	cd SOIL
	cmake . -G "Unix Makefiles"
	make -j10 && sudo make install 

Awesomium is only distributed in binary form.  Download and run the installer from the [Awesomium website](http://www.awesomium.com/). 

Once all the dependencies are installed, you can build Sigma.

```sh
mkdir build/
cd build/
cmake ..
make
```

Running
===

__(Unix/Linux/OS X)__

The binary `Sigma` is written to `build/bin`, `build/bin/debug`, or `build/debug/bin`

On OS X, you must launch Sigma from a shell in the build directory.  Launching Sigma through the Finder will not set the working directory correctly.

Visual Studio
===
You must change the startup project via right-clicking on the project in the solution explorer in order for Debugging to launch the correct program.
Also you must go into the project's properties, and under the 'Debugging' category change the 'Working Directory' to '$(OutDir)'.

Xcode
===
On OS X, CMake can generate an Xcode project for Sigma.

```sh
mkdir build/
cd build/
cmake .. -G Xcode
```

You must change the current scheme to Sigma by clicking on the scheme popup menu and selecting Sigma.  You can also change the working directory used when Sigma is started by Xcode.  Select Edit Scheme from the scheme popup menu, switch to the Options tab, check the box next to Working Directory, and enter the path to the Sigma assets in the text field.
