# BitPresent
## A presentation engine, that creates a PDF from a markdown styled input file

### Installation
Installation is straight forward, just download the package you need from the releases and either extract the folder or run the installer if you are on Windows.

**Important!** Make sure to have a folder called ".cache" in the installation folder. If not, the program will crash. You can check by using following commands in the terminal:
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
git clone httpps://github.com/bittim/bitpresent/BitPresent
cd BitPresent
make init
make
```

#### Windows:
/Work in Progress/
