#include "lib/init.hh"

#include <filesystem>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "lib/globals.hh"
#include "lib/objects.hh"
#include "lib/loader.hh"

namespace fs = std::filesystem;

fs::path Global::_CACHEPATH;
int Global::_NEXTID = 10001;

void initCache()
{
	Global::_CACHEPATH = fs::temp_directory_path();
	fs::create_directories(Global::_CACHEPATH / "BitPresent/Editor/Images");
	fs::create_directory(Global::_CACHEPATH / "BitPresent/Editor/Fonts");
	Global::_CACHEPATH = Global::_CACHEPATH / "BitPresent" / "";
}

void initSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP);
	TTF_Init();
}

bool initDefaults(Presentation &present)
{
	Default::_BACKGROUND = "none";
	Default::_COLOR = new SDL_Color{ 33, 33, 33 };
	return loadFont(Default::_FONT);
}

void initPresent(Presentation &present)
{
	present.background = Default::_BACKGROUND;
	present.textcolor = Default::_COLOR;
	present.font = Default::_FONT;
}

bool init(Presentation &present)
{
	initCache();
	initSDL();
	initPresent(present);
	return initDefaults(present);
}