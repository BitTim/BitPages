#include <iostream>

#include "lib/globals.hh"
#include "lib/fileParser.hh"
#include "lib/imageGenerator.hh"

//Initialize all global variables
int Global::_WIDTH = 1280;
int Global::_HEIGHT = 720;
int Global::_BORDERS = 20;

Presentation* Global::_PRESENT = nullptr;
int Global::_CSLIDE = -1;

std::map<std::string, TTF_Font*> Global::_FONT;
std::map<std::string, TTF_Font*> Global::_DEFAULTFONT;
SDL_Color* Global::_TEXTCOLOR = nullptr;
SDL_Color* Global::_DEFAULTTEXTCOLOR = nullptr;
std::string Global::_BACKGROUND = "";
std::string Global::_DEFAULTBACKGROUND = "";

int main(int argc, char* argv[])
{
	//Check Argument length
	if (argc != 2)
	{
		printf("[USAGE]: bitPresent <path>\n");
		return 0;
	}

	//Initialize SDL for future Ops
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	TTF_Init();

	//Load default font
	Global::_DEFAULTFONT = {{"title", TTF_OpenFont("dat/defaultFont.ttf", 68)}, {"subtitle", TTF_OpenFont("dat/defaultFont.ttf", 46)}, {"normal", TTF_OpenFont("dat/defaultFont.ttf", 34)}};
	Global::_FONT = Global::_DEFAULTFONT;

	//Crate default Textcolor
	Global::_DEFAULTTEXTCOLOR = new SDL_Color {33, 33, 33};
	Global::_TEXTCOLOR = Global::_DEFAULTTEXTCOLOR;

	//Create default Background
	Global::_DEFAULTBACKGROUND = "none";
	Global::_BACKGROUND = Global::_DEFAULTBACKGROUND;

	//Split input file into tokens
	std::vector<std::string> tokens = tokenize(argv[1]);
	if (tokens.size() == 0)
	{
		printf("[ERROR]: Error while opening file\n");
	}

	//Create global Presentation object
	Global::_PRESENT = new Presentation();

	//Parse tokens to global Presentation object
	parse(tokens);

	//Generate images for each Slide
	for(int i = 0; i < Global::_PRESENT->slides.size(); i++)
	{
		SDL_Surface* surface = generateSurface(i);
		saveImage(surface, ".cache/" + std::to_string(i) + ".png");
		SDL_FreeSurface(surface);
	}

	printf("[OK]: Created all Images\n");

	//Create new PDF Document
	

	//Exit SDL
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}