#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "objects.hh"
#include "globals.hh"

SDL_Surface* generateSurface(int slide)
{
  SDL_Surface* surface = SDL_CreateRGBSurface(0, Global::_WIDTH, Global::_HEIGHT, 32, 0, 0, 0, 0);

  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
  if(Global::_PRESENT->background != "none" && Global::_PRESENT->background != "default")
  {
    //SDL_Surface* image = IMG_Load(Global::_PRESENT->background.c_str());
    SDL_Surface* image = IMG_Load("/home/tim/Coding/Sample Files/sampleImage.png");
    printf("[DEBUG]: Background path: %s\n", Global::_PRESENT->background.c_str());
    if(image == NULL) printf("[ERROR]: %s\n", SDL_GetError());
    else SDL_BlitSurface(image, NULL, surface, NULL);
  }



  return surface;
}

void saveImage(SDL_Surface* surface, std::string path)
{
  IMG_SavePNG(surface, path.c_str());
}