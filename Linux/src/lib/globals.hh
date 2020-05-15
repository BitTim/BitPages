#pragma once
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

#include "objects.hh"
#include "gui.hh"

struct Global
{
    static std::string _VERSIONSTRING;

    static int _WIDTH;
    static int _HEIGHT;
    static int _BORDERS;
    static int _INDENT;

    static bool useGUI;
    static guiApp* gApp;

    static std::vector<Presentation*> _PRESENT;
    static int _CPRESENT;
    static int _CSLIDE;
    static int _CPOINT;

    static std::map<std::string, TTF_Font*> _FONT;
    static std::map<std::string, TTF_Font*> _DEFAULTFONT;
    static SDL_Color* _TEXTCOLOR;
    static SDL_Color* _DEFAULTTEXTCOLOR;
    static std::string _BACKGROUND;
    static std::string _DEFAULTBACKGROUND;

    static std::string _STATUS;
    static int _MAXPROGRESS;
    static int _PROGRESS;
};
