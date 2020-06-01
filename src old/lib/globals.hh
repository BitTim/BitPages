#pragma once
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <filesystem>

#define _VERSIONSTRING "BitPresent v1.3.3"
#define _LICENSE "GPL3 License: https://www.gnu.org/licenses/gpl-3.0.en.html"

#define _WIDTH 1920
#define _HEIGHT 1080
#define _BORDER 20
#define _INDENT 40

namespace fs = std::filesystem;

struct Global
{
    static fs::path _CACHEPATH;
};

struct Default
{
    static std::map<std::string, TTF_Font*> _FONT;
    static SDL_Color* _COLOR;
    static std::string _BACKGROUND;
};