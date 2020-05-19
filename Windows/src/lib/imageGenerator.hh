#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

extern SDL_Surface* generateSurface(int slide);
extern void saveImage(SDL_Surface* surface, std::string path);