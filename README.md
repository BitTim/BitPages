# BitPresent
## An interpreter for creation of presentations and documents in PDF format, based on Markdown styled input

### Installation
Installation is straight forward, just download the package you need from the releases and either extract the folder or run the installer if you are on Windows.

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
git clone https://github.com/bittim/BitPresent
cd BitPresent
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

Open the Visual Studio Solution (BitPresent.sln) in "visual studio files" and build with the Release x86 Configuration
