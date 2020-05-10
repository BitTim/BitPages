#include "lib/presentCreator.hh"

#include "lib/globals.hh"
#include "lib/fileParser.hh"
#include "lib/imageGenerator.hh"
#include "lib/pdfCreator.hh"
#include "lib/gui.hh"

int createPresent(std::string inpath, std::string outpath)
{
  printf("%s\n", Global::_VERSIONSTRING.c_str());
	if(Global::useGUI)
	{
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0);
		gtk_text_buffer_set_text(buffer, (Global::_VERSIONSTRING + "\n").c_str(), -1);
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

	//Parse tokens to global Presentation object
	parse(tokens);
	gprintf("[OK]: Finished Parsing\n");
	if(Global::useGUI) gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.34);

	//Generate images for each Slide
	for (int i = 0; i < Global::_PRESENT->slides.size(); i++)
	{
		SDL_Surface* surface = generateSurface(i);
		if (surface == nullptr) return -1;

		saveImage(surface, ".cache/" + std::to_string(i) + ".png");
		SDL_FreeSurface(surface);
	}

	gprintf("[OK]: Created all Images\n");
	if(Global::useGUI) gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 0.67);

	//Create PDF
	createPDF(outpath);
	gprintf("[OK]: Created PDF file\n");
	if(Global::useGUI) gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), 1);

	//Cleanup
	delete Global::_PRESENT;

	gprintf("[FINISHED]: Done creating presentation\n");

	return 0;
}