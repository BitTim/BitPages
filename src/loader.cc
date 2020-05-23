#include "lib/loader.hh"

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>

bool loadFont(std::map<std::string, TTF_Font*> &font)
{
	font = {
		{"title", TTF_OpenFont("dat/defaultFont.ttf", 68)},
		{"subtitle", TTF_OpenFont("dat/defaultFont.ttf", 46)},
		{"normal", TTF_OpenFont("dat/defaultFont.ttf", 34)},
		{"footer", TTF_OpenFont("dat/defaultFont.ttf", 12)}
	};

	if (std::find(font.begin(), font.end(), NULL) != font.end())
	{
		printf("[ERROR]: Loading font: %s\n", TTF_GetError());
		return false;
	}

	return true;
}