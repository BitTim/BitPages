#pragma once
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <filesystem>

#include "objects.hh"
#include "parser.hh"
#include "gui.hh"

namespace fs = std::filesystem;

struct Global
{
    static std::string _VERSIONSTRING;

    static bool _SAVED;
    static std::string _SAVEPATH;
    static std::string _EXPORTPATH;

    static int _WIDTH;
    static int _HEIGHT;
    static int _BORDERS;
    static int _INDENT;

    static bool useGUI;
    static EditorGUIApp* gApp;
    static fs::path _CACHEPATH;

    static Presentation* _PRESENT;
    static int _CPRESENT;
    static int _CSLIDE;
    static int _CPOINT;
    static int _PREVSLIDEPREVIEW;
    static int _CSLIDEPREVIEW;
    static bool _FORCEUPDATE;

    static std::map<std::string, TTF_Font*> _DEFAULTFONT;
    static SDL_Color* _DEFAULTTEXTCOLOR;
    static std::string _DEFAULTBACKGROUND;

    static std::vector<ErrorHighlight> _ERRORS;
};
