#pragma once
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

#include "objects.hh"

struct Global
{
    static std::string _VERSIONSTRING;

    static int _WIDTH;
    static int _HEIGHT;
    static int _BORDERS;
    static int _INDENT;

    static std::string _OUTPATH;

    static Presentation* _PRESENT;
    static int _CSLIDE;
    static int _CPOINT;

    static std::map<std::string, TTF_Font*> _FONT;
    static std::map<std::string, TTF_Font*> _DEFAULTFONT;
    static SDL_Color* _TEXTCOLOR;
    static SDL_Color* _DEFAULTTEXTCOLOR;
    static std::string _BACKGROUND;
    static std::string _DEFAULTBACKGROUND;
};