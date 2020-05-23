# BitPresent
## A presentation engine, that creates a PDF from a markdown styled input file

### Installation
Installation is straight forward, just download the package you need from the releases and either extract the folder or run the installer if you are on Windows.

**Important!** Make sure to have a folder called ".cache" for Linux or "cache" for Windows in the installation folder. If not, the program will crash. For Linux you can check by using following commands in the terminal:
```
cd path/to/bitPresent/installation
ls -a
```

### Usage:
See the documentation in the docs folder here or just follow this link:
https://github.com/BitTim/BitPresent/blob/master/docs/BitPresent%20-%20Documentation.pdf

### Compiling
#### Linux:
Make sure to have following installed:
* gcc
* libsdl2 (libsdl2-dev on Ubuntu based)
* libsdl2-ttf (libsdl2-ttf-dev on Ubuntu based)
* libsdl2-image (libsdl2-image-dev on Ubuntu based)
* libharu
* wxgtk3 (or wxwidgets in general)

And their dependencies. After that, run following commands:

```
git clone https://github.com/bittim/bitpresent/BitPresent
cd BitPresent/Linux/
make init
make
```

#### Windows:
Install Visual Studio and vcpkg.
Install following through vcpkg:
* sdl2
* sdl2-ttf
* sdl2-image
* sdl2-image[libjpeg-turbo]
* sdl2-image[libwebp]
* libharu
* wxwidgets

Create a new empty Project, import the source files (Located in ./src) and modify following project settings for the Release config:
* Configuration Properties -> General -> C++ Language Standard -> ISO C++17 Standard (std:c++17)
* C/C++ -> Preprocessor -> Preprocessor Definitions -> Add "WIN32;NDEBUG;_WINDOWS;_CONSOLE;%(PreprocessorDefinitions);_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;WINVER=0x0400;__WXMSW__;wxUSE_GUI=1;WXUSINGDLL
* Linker -> Input -> Additional Dependencies -> Add "manual-link/SDL2main.lib;SDL2.lib;SDL2_ttf.lib;SDL2_image.lib;libhpdf.lib;wxmsw31u_core.lib;wxbase31u.lib;comctl32.lib;rpcrt4.lib;winmm.lib;advapi32.lib;wsock32.lib;%(AdditionalDependencies)"
* Linker -> System -> SubSystem -> Windows (/SUBSYSTEM:WINDOWS)
Build with the Release config selected