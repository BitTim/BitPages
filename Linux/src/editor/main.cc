#include <SDL2/SDL_image.h>

#include "lib/globals.hh"
#include "lib/gui.hh"

#define EDITOR

//Initialize all global variables
std::string Global::_VERSIONSTRING = "BitPresentEditor v1.0.0";

int Global::_WIDTH = 1920;
int Global::_HEIGHT = 1080;
int Global::_BORDERS = 20;
int Global::_INDENT = 40;

Presentation* Global::_PRESENT;
int Global::_CPRESENT = -1;
int Global::_CSLIDE = -1;
int Global::_CPOINT = -1;

bool Global::useGUI = true;

bool Global::_SAVED = true;
std::string Global::_SAVEPATH = "";
std::string Global::_EXPORTPATH = "";

std::map<std::string, TTF_Font*> Global::_FONT;
std::map<std::string, TTF_Font*> Global::_DEFAULTFONT;
SDL_Color* Global::_TEXTCOLOR = nullptr;
SDL_Color* Global::_DEFAULTTEXTCOLOR = nullptr;
std::string Global::_BACKGROUND = "";
std::string Global::_DEFAULTBACKGROUND = "";

EditorGUIApp* Global::gApp = nullptr;
std::string Global::_INPATH = "";
fs::path Global::_CACHEPATH = fs::temp_directory_path();

int main(int argc, char* argv[])
{
  //Create Foled in /tmp
	fs::create_directory(Global::_CACHEPATH / "BitPresent");
	Global::_CACHEPATH = Global::_CACHEPATH / "BitPresent" / "";

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

  //Init and enter GUI
	Global::gApp = new EditorGUIApp(); 
	wxApp::SetInstance(Global::gApp);
	wxEntry(argc, argv);
  wxEntryCleanup();

	//Clear /tmp folder
	fs::remove_all(Global::_CACHEPATH);
	fs::remove(Global::_CACHEPATH);

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
