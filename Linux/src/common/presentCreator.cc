#include "../common/lib/presentCreator.hh"

#ifdef EDITOR
    #include "../editor/lib/globals.hh"
    #include "../editor/lib/gui.hh"
		#include "../editor/lib/parser.hh"
#else
    #include "../compiler/lib/globals.hh"
    #include "../compiler/lib/gui.hh"
		#include "../compiler/lib/fileParser.hh"
#endif

#include "../common/lib/imageGenerator.hh"
#include "../common/lib/pdfCreator.hh"

int createPresent(std::string inpath, std::string outpath)
{
  printf("%s\n", Global::_VERSIONSTRING.c_str());
	if(Global::useGUI)
	{
		resetProgress();
		clearTerminal();
		changeStatus("Parsing");
	}
  
  //Split input file into tokens
	std::vector<std::string> tokens = tokenize(inpath);
	if (tokens.size() == 0)
	{
		gprintf("[ERROR]: Error while opening file\n");
		return -1;
	}

	//Create global Presentation object
	Global::_PRESENT = new Presentation();
	Global::_CPRESENT += 1;

	//Parse tokens to global Presentation object
	parse(tokens);
	gprintf("[OK]: Finished Parsing\n");
	if(Global::useGUI)
	{
		changeStatus("Creating Images");
		progress();
	}
	tokens.clear();

	//Generate images for each Slide
	for (int i = 0; i < Global::_PRESENT->slides.size(); i++)
	{
		SDL_Surface* surface = generateSurface(i);
		if (surface == nullptr) return -1;

		saveImage(surface, Global::_CACHEPATH.string() + std::to_string(i) + ".png");
		SDL_FreeSurface(surface);
	}

	gprintf("[OK]: Created all Images\n");
	if(Global::useGUI)
	{
		changeStatus("Generating PDF");
		progress();
	}

	//Create PDF
	createPDF(outpath);
	gprintf("[OK]: Created PDF file\n");
	if(Global::useGUI) changeStatus("Finished");

	//Cleanup
	Global::_PRESENT->clean();
	Global::_CSLIDE = -1;
	Global::_CPOINT = -1;

	gprintf("[FINISHED]: Done creating presentation\n");
	return 0;
}