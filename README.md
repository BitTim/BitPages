# BitPresent
## A presentation engine, that creates a PDF from a markdown styled input file

### Installation
Installation is straight forward, just download the package you need from the releases and either extract the folder or run the installer if you are on Windows.

**Important!** Make sure to have a folder called ".cache" in the installation folder. If not, the program will crash. For Linux you can check by using following commands in the terminal:
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
And their dependencies. After that, run following commands:

```
git clone https://github.com/bittim/bitpresent/BitPresent
cd BitPresent
make init
make
```

#### Windows:
You need either a Linux machine or MSYS2.
After you have set either of those up, install the following:
* mingw-w64-gcc
* mingw-w64-sdl2
* mingw-w64-sdl2_ttf
* mingw-w64-sdl2_image
* mingw-w64-libharu

After that run the following commands:
```
git clone https://github.com/bittim/bitpresent/BitPresent
cd BitPresent
make init
make PRODUCT=BitPresent.exe CXX=x86_64-w64-mingw32-g++
```
