#include <iostream>

#include "lib/globals.hh"
#include "lib/fileParser.hh"
#include "lib/imageGenerator.hh"

//Initialize all global variables
int Global::_WIDTH = 1280;
int Global::_HEIGHT = 720;

Presentation* Global::_PRESENT = nullptr;
int Global::_CSLIDE = -1;

TTF_Font* Global::_FONT = nullptr;
TTF_Font* Global::_DEFAULTFONT = nullptr;

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
	Global::_DEFAULTFONT = TTF_OpenFont("dat/defaultFont.ttf", 16);
	Global::_FONT = Global::_DEFAULTFONT;

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
		saveImage(generateSurface(i), ".cache/" + std::to_string(i) + ".png");
	}

	return 0;
}