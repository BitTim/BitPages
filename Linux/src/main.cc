#include <iostream>

#include "lib/globals.h"
#include "lib/fileParser.h"
#include "lib/imageGenerator.h"
#include "lib/pdfCreator.h"

//Initialize all global variables
std::string Global::_VERSIONSTRING = "BitPresent v1.1.0";

int Global::_WIDTH = 1920;
int Global::_HEIGHT = 1080;
int Global::_BORDERS = 20;
int Global::_INDENT = 40;

std::string Global::_OUTPATH = "";

Presentation* Global::_PRESENT = nullptr;
int Global::_CSLIDE = -1;
int Global::_CPOINT = -1;

std::map<std::string, TTF_Font*> Global::_FONT;
std::map<std::string, TTF_Font*> Global::_DEFAULTFONT;
SDL_Color* Global::_TEXTCOLOR = nullptr;
SDL_Color* Global::_DEFAULTTEXTCOLOR = nullptr;
std::string Global::_BACKGROUND = "";
std::string Global::_DEFAULTBACKGROUND = "";

int main(int argc, char* argv[])
{
	std::string inpath = "";

	//Check Argument length
	if (argc > 3)
	{
		printf("[ERROR]: Too many arguments\n");
		return -1;
	}

	printf("%s\n", Global::_VERSIONSTRING.c_str());

	if (argc < 2)
	{
		printf("[USAGE]: bitPresent <input_path> [<output_path>]\n\n");
		printf("Please enter the input file path: ");
		getline(std::cin, inpath);
		printf("Please enter the output file path: ");
		getline(std::cin, Global::_OUTPATH);
	}
	else
	{
		inpath = argv[1];
		if (argc == 3) Global::_OUTPATH = argv[2];
		else Global::_OUTPATH = "presentation.pdf";
	}

	//Initialize SDL for future Ops
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP);
	TTF_Init();

	//Load default font
	Global::_DEFAULTFONT = { {"title", TTF_OpenFont("dat/defaultFont.ttf", 68)}, {"subtitle", TTF_OpenFont("dat/defaultFont.ttf", 46)}, {"normal", TTF_OpenFont("dat/defaultFont.ttf", 34)}, {"footer", TTF_OpenFont("dat/defaultFont.ttf", 12)} };
	if (Global::_DEFAULTFONT["title"] == NULL || Global::_DEFAULTFONT["subtitle"] == NULL || Global::_DEFAULTFONT["normal"] == NULL)
	{
		printf("[ERROR]: Loading font: %s\n", TTF_GetError());
		return -1;
	}
	Global::_FONT = Global::_DEFAULTFONT;

	//Crate default Textcolor
	Global::_DEFAULTTEXTCOLOR = new SDL_Color{ 33, 33, 33 };
	Global::_TEXTCOLOR = Global::_DEFAULTTEXTCOLOR;

	//Create default Background
	Global::_DEFAULTBACKGROUND = "none";
	Global::_BACKGROUND = Global::_DEFAULTBACKGROUND;

	//Split input file into tokens
	std::vector<std::string> tokens = tokenize(inpath);
	if (tokens.size() == 0)
	{
		printf("[ERROR]: Error while opening file\n");
		return -1;
	}

	//Create global Presentation object
	Global::_PRESENT = new Presentation();

	//Parse tokens to global Presentation object
	parse(tokens);

	//Generate images for each Slide
	for (int i = 0; i < Global::_PRESENT->slides.size(); i++)
	{
		SDL_Surface* surface = generateSurface(i);
		if (surface == nullptr) return -1;

		saveImage(surface, "cache/" + std::to_string(i) + ".png");
		SDL_FreeSurface(surface);
	}

	printf("[OK]: Created all Images\n");

	//Create PDF
	createPDF();
	printf("[OK]: Created PDF file\n");

	//Clear Fonts
	TTF_CloseFont(Global::_FONT["title"]);
	TTF_CloseFont(Global::_FONT["subtitle"]);
	TTF_CloseFont(Global::_FONT["normal"]);

	//Exit SDL
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	printf("[FINISHED]: Done creating presentation\n");

	return 0;
}