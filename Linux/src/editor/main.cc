#include <SDL2/SDL_image.h>

#include "lib/globals.hh"
#include "lib/gui.hh"

//Initialize all global variables
std::string Global::_VERSIONSTRING = "BitPresent Editor v1.0.0";

int Global::_WIDTH = 1920;
int Global::_HEIGHT = 1080;
int Global::_BORDERS = 20;
int Global::_INDENT = 40;

Presentation* Global::_PRESENT;
int Global::_CPRESENT = -1;
int Global::_CSLIDE = -1;
int Global::_CPOINT = -1;
int Global::_PREVSLIDEPREVIEW = -1;
int Global::_CSLIDEPREVIEW = 0;

bool Global::useGUI = true;
bool Global::_FORCEUPDATE = false;

bool Global::_SAVED = true;
std::string Global::_SAVEPATH = "";
std::string Global::_EXPORTPATH = "";

std::map<std::string, TTF_Font*> Global::_DEFAULTFONT;
SDL_Color* Global::_DEFAULTTEXTCOLOR = nullptr;
std::string Global::_DEFAULTBACKGROUND = "";

EditorGUIApp* Global::gApp = nullptr;
fs::path Global::_CACHEPATH = fs::temp_directory_path();

std::vector<ErrorHighlight> Global::_ERRORS;
std::thread Global::_PREVIEWTHREAD;

bool Global::_CREATEDPREVIEWIMAGE = false;
bool Global::_LOCKPREVIEWIMAGE = false;

int main(int argc, char* argv[])
{
  //Create Foled in /tmp
	fs::create_directories(Global::_CACHEPATH / "BitPresent/Editor/Images");
	fs::create_directory(Global::_CACHEPATH / "BitPresent/Editor/Fonts");
	Global::_CACHEPATH = Global::_CACHEPATH / "BitPresent" / "";

	//Initialize SDL for future Ops
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP);
	TTF_Init();

	//Create Presentation Object for later
	Global::_PRESENT = new Presentation();

	//Crate default Textcolor
	Global::_DEFAULTTEXTCOLOR = new SDL_Color{ 33, 33, 33 };
	Global::_PRESENT->textcolor = Global::_DEFAULTTEXTCOLOR;

	//Create default Background
	Global::_DEFAULTBACKGROUND = "none";
	Global::_PRESENT->background = Global::_DEFAULTBACKGROUND;

  //Init and enter GUI
	Global::gApp = new EditorGUIApp(); 
	wxApp::SetInstance(Global::gApp);
	wxEntry(argc, argv);
  wxEntryCleanup();

	//Clear /tmp folder
	fs::remove_all(Global::_CACHEPATH / "Editor/Images/");
	fs::remove_all(Global::_CACHEPATH / "Editor/Fonts/");
	fs::remove_all(Global::_CACHEPATH / "Editor/");
	fs::remove_all(Global::_CACHEPATH);
	fs::remove(Global::_CACHEPATH);

	//Clear Fonts
	TTF_CloseFont(Global::_DEFAULTFONT["title"]);
	TTF_CloseFont(Global::_DEFAULTFONT["subtitle"]);
	TTF_CloseFont(Global::_DEFAULTFONT["normal"]);

	//Exit SDL
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}