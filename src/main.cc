#include <iostream>

#include "lib/globals.hh"
#include "lib/fileParser.hh"
#include "lib/imageGenerator.hh"
#include "lib/pdfCreator.hh"

//Initialize all global variables
int Global::_WIDTH = 1280;
int Global::_HEIGHT = 720;
int Global::_BORDERS = 20;

std::string Global::_OUTPATH = "";

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
	if (argc < 2 || argc > 3)
	{
		printf("[USAGE]: bitPresent <input_path> [<output_path>]\n");
		return 0;
	}

	if(argc == 3) Global::_OUTPATH = argv[2];
	else Global::_OUTPATH = "presentation.pdf";

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

	//Create PDF
	createPDF();

	//Clear Fonts
	TTF_CloseFont(Global::_FONT["title"]);
	TTF_CloseFont(Global::_FONT["subtitle"]);
	TTF_CloseFont(Global::_FONT["normal"]);

	//Exit SDL
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}