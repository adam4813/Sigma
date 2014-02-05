Sigma
===

The Sigma game engine for creating games on multiple platforms. The name Sigma was chosen due to its meaning in math of summation notation. In Sigma, engine components are assigned to entities, and it is the sum of all the components that makes an entity.

Requires CMake and one of the following compilers:

* GCC 4.8 or newer;
* Clang 3.3 or newer (Xcode 5.0 or newer on OS X);
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
* [Chromium Embedded Framework](http://code.google.com/p/chromiumembedded) 3;
* An OpenAL API implementation;
* [libogg](https://www.xiph.org/ogg/);
* [libvorbis](https://www.xiph.org/ogg/);

Sigma on Linux also requires [GTK+ 2](http://www.gtk.org), due to usage of Chromium Embedded Framework.
When building these libraries from source, always build them as shared objects (`.so`). In cmake-builds, this can be done using `cmake -DBUILD_SHARED_LIBS=ON`.

You'll also need a [package of assets](http://wiki.trillek.org/wiki/Assets).  Unpack it in the build/bin/ directory.

## Setting up Chromium Embedded Framework ###
Make sure you use a binary release from [Adobe](http://www.cefbuilds.com). **For Windows and Linux, use the latest version that is NOT marked as dev (trunk).  For OS X, use the latest Mac 32-bit from dev (trunk)**

__(Windows/Linux)__

1. Unzip the downloaded tarball.
2. This step depends on your platform. **Make sure to use the Release build mode.**
    * __Windows__ build the included `libcef_dll_wrapper.vcxproj` project
    * __Linux__ run `make libcef_dll_wrapper BUILDTYPE=Release`
3. Make `cef/` and `cef/bin/` directories in `Sigma/`
4. Copy the `out/Release/obj.target/libcef_dll_wrapper` directory, if any, and the contents of `Resources/` directories into `Sigma/cef/bin/`
5. Copy the contents of include/ directory into Sigma's include/.
6. Copy `out/Release/obj.target/libcef_dll_wrapper.a` (or .lib) to `Sigma/cef/`
7. This step also depends on your platform.
    * __Windows__ copy all the .dll files in `Release/` to `Sigma/cef/bin`, then copy the .lib file into `Sigma/cef/`
    * __Linux__ copy the entire contents of `Release/` into `Sigma/cef/bin/`
8. On Linux, make a symlink pointing to libcef.so in the `cef/` directory.
9. Make a symlink pointing to `Sigma/cef/bin/libcef_dll_wrapper` in the `cef/` directory. `libcef_dll_wrapper` and `libcef_dll_wrapper.a` must appear in the same place, otherwise you will get a "Malformed Archive" error when linking.

- If you get "Multiply defined symbols" errors building Sigma on Windows with Visual Studio, then make these changes in the libcef_dll_wrapper properties: C/C++ / Preprocessor, edit the definitions to set `_HAS_EXCEPTIONS=1` instead of 0; under C/C++ / Code generation, select the /MDd or /MD runtime library; rebuild and copy the libcef_dll_wrapper.lib to `cef/` directory.


__(OS X)__

1. Unzip the downloaded tarball into the Sigma folder.  Name it `cef`.
2. From a shell in the Sigma directory, use the following commands to build CEF.

		xcodebuild -project cef/cefclient.xcodeproj -target All -configuration Release -sdk macosx MACOSX_DEPLOYMENT_TARGET=10.7 CLANG_CXX_LANGUAGE_STANDARD=c++11 CLANG_CXX_LIBRARY=libc++

## Building ##

Use CMake to create makefiles or projects. The resulting executable will be saved in build/bin/.

## OS X ##

If you want to compile Sigma on OS X, your system must be running OS X 10.7 or later.  You will need to have installed Xcode and, unless your system is running 10.9, the Command Line Tools.  Both can be downloaded from the [Apple Developer Website](https://developer.apple.com/downloads).  

Before you can build Sigma, you'll first need to install the dependencies separately.  Using a package manager, such as [MacPorts](http://macports.org), is recommended.

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
