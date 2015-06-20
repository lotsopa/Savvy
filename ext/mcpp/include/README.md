# mcpp

This mcpp library is a fork of [Ice] (https://github.com/zeroc-ice/ice), which in turn is a fork of [mcpp](http://mcpp.sourceforge.net/). Mcpp is a fast and complete C/C++ Preprocessor and is used in Savvy. 
This fork includes all bug-fixed and improvements from the [Ice] fork and more. It also implements some extra functionality. All credit for the build system described in this README goes to [Ice].

.

## Build Instructions

### Linux, OS X and Windows with MinGW
Simply type:
```
  make
```  

This creates a static library `libmcpp.a`. Depending on your platform, the library is created in the following directory:

* `lib/i386-linux-gnu` - Ubuntu 32-bit
* `lib/x86_64-linux-gnu` - Ubuntu 64-bit
* `lib64` - Any Linux system with a `/usr/lib64` directory
* `lib` - OS X, Windows with MinGW, and other platforms

### Windows with Visual Studio

Open a Visual Studio Command Prompt, and type:
```
  nmake -f Makefile.mak
```
This creates two static libraries: `mcpp.lib`, built with `/MD` ("release") and `mcppd.lib`, built with `/MDd` ("debug").
