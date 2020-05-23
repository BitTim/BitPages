#include "lib/cleanup.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "lib/globals.hh"
#include "lib/objects.hh"

void cleanCache()
{
	fs::remove_all(Global::_CACHEPATH / "Editor/Images/");
	fs::remove_all(Global::_CACHEPATH / "Editor/Fonts/");
	fs::remove_all(Global::_CACHEPATH / "Editor/");
	fs::remove_all(Global::_CACHEPATH);
}

void cleanFonts()
{
	TTF_CloseFont(Default::_FONT["title"]);
	TTF_CloseFont(Default::_FONT["subtitle"]);
	TTF_CloseFont(Default::_FONT["normal"]);
	TTF_CloseFont(Default::_FONT["footer"]);
}

void cleanSDL()
{
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void clean(Presentation &present)
{
	present.clean();

	cleanCache();
	cleanFonts();
	cleanSDL();
}