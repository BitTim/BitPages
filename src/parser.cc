#include <algorithm>
#include <vector>
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;

#include "lib/parser.hh"
#include "lib/objects.hh"
#include "lib/gui.hh"
#include "lib/globals.hh"

int nDotWarn = 0;

ErrorHighlight::ErrorHighlight(int type, int line, std::string desc)
{
	this->type = type;
	this->line = line;
	this->desc = desc;
}

std::vector<Token> tokenize(std::string input)
{
	Global::_ERRORS.clear();
	Global::_PRESENT->clean();
	Global::_CSLIDE = -1;
	Global::_CPOINT = -1;
	
	std::vector<Token> tokens;

	std::stringstream in(input);
	if (in.fail()) return tokens;

	std::string line;
	std::string buffer;
	int cline = 0;

	while (std::getline(in, line))
	{
		cline += 1;
		if (line[0] == '.')
		{
			int index = line.find(" ");
			if (index == std::string::npos)
			{
				Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Invalid '.' command statement"));
				nDotWarn++;
				continue;
			}
			else
			{
				tokens.push_back(Token(line.substr(0, index), true));
				tokens.push_back(Token(line.substr(index + 1, line.length() - 1)));
			}
		}
		else
		{
			tokens.push_back(Token(line));
		}
	}

	return tokens;
}

void parse(std::vector<Token> tokens, int cursorLine)
{
	int cline = nDotWarn;

	if(Global::_PRESENT == nullptr)
	{
		Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, 0, "Presentation object does not exist"));
		return;
	}

	for (int i = 0; i < tokens.size(); i++)
	{
		cline += 1;

		//Skip empty tokens
		if (tokens[i].value == "") continue;

		//Check for commands
		if (tokens[i].value == ".background")
		{
			i += 1;
			if (tokens[i].value[0] == '<' || tokens[i].value[0] == '[' || tokens[i].value[0] == '-') Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Using instruction as argument"));
			Global::_PRESENT->background = tokens[i].value;

			if(fs::path(Global::_PRESENT->background).is_relative())
      {
        std::string tmp;
        tmp = Global::_SAVEPATH + "/" + Global::_PRESENT->background;
        Global::_PRESENT->background = tmp;
      }

			if(!fs::exists(Global::_PRESENT->background)) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, cline, "File does not exist"));
		}

		else if (tokens[i].value == ".image")
		{
			i += 1;
			if (tokens[i].value[0] == '<' || tokens[i].value[0] == '[' || tokens[i].value[0] == '-') Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Using instruction as argument"));
			if (Global::_CSLIDE < 0) Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Defined image outside of a slide"));
			else Global::_PRESENT->slides[Global::_CSLIDE].image = tokens[i].value;

			if(fs::path(Global::_PRESENT->slides[Global::_CSLIDE].image).is_relative())
      {
        std::string tmp;
        tmp = Global::_SAVEPATH + "/" + Global::_PRESENT->slides[Global::_CSLIDE].image;
        Global::_PRESENT->slides[Global::_CSLIDE].image = tmp;
      }

      if(!fs::exists(Global::_PRESENT->slides[Global::_CSLIDE].image)) Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, cline, "File does not exist"));
		}

		else if (tokens[i].value == ".font")
		{
			i += 1;
			if (tokens[i].value[0] == '<' || tokens[i].value[0] == '[' || tokens[i].value[0] == '-') Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Using instruction as argument"));
			
			if(fs::path(tokens[i].value).is_relative())
			{
				std::string tmp;
				tmp = Global::_SAVEPATH + "/" + tokens[i].value;
				tokens[i].value = tmp;
			}
			
			Global::_PRESENT->font = { {"title", TTF_OpenFont(tokens[i].value.c_str(), 68)}, {"subtitle", TTF_OpenFont(tokens[i].value.c_str(), 50)}, {"normal", TTF_OpenFont(tokens[i].value.c_str(), 34)}, {"footer", TTF_OpenFont(tokens[i].value.c_str(), 12)} };
			if (Global::_PRESENT->font["title"] == NULL || Global::_PRESENT->font["subtitle"] == NULL || Global::_PRESENT->font["normal"] == NULL || Global::_PRESENT->font["footer"] == NULL)
			{
				Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, cline, "Error loading font, reverting to default"));
				Global::_PRESENT->font = Global::_DEFAULTFONT;
			}
		}

		else if (tokens[i].value == ".color")
		{
			i += 1;
			if(tokens[i].value == "default") Global::_PRESENT->textcolor = Global::_DEFAULTTEXTCOLOR;
			else if(tokens[i].value.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos && tokens[i].value.length() == 6)
			{
				uint8_t r = std::stoi(tokens[i].value.substr(0, 2), nullptr, 16);
				uint8_t g = std::stoi(tokens[i].value.substr(2, 2), nullptr, 16);
				uint8_t b = std::stoi(tokens[i].value.substr(4, 2), nullptr, 16);

				Global::_PRESENT->textcolor = new SDL_Color{ r, g, b };
			}
			else
			{
				Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, cline, "Invalid Color code. Reverting to default"));
				Global::_PRESENT->textcolor = Global::_DEFAULTTEXTCOLOR;
			}
		}

		//Check for other defining Tokens
		else if (tokens[i].value[0] == '<' && tokens[i].value[tokens[i].value.length() - 1] == '>')
		{
			Global::_PRESENT->slides.push_back(Slide());
			Global::_CSLIDE = Global::_CSLIDE + 1;
			Global::_CPOINT = -1;

			Global::_PRESENT->slides[Global::_CSLIDE].titleSlide = true;

			std::string tmp = tokens[i].value.substr(1, tokens[i].value.length() - 2);
			Global::_PRESENT->slides[Global::_CSLIDE].title = tmp;
		}

		else if (tokens[i].value[0] == '[' && tokens[i].value[tokens[i].value.length() - 1] == ']')
		{
			Global::_PRESENT->slides.push_back(Slide());
			Global::_CSLIDE = Global::_CSLIDE + 1;
			Global::_CPOINT = -1;

			std::string tmp = tokens[i].value.substr(1, tokens[i].value.length() - 2);
			Global::_PRESENT->slides[Global::_CSLIDE].title = tmp;
		}

		else if (tokens[i].value[0] == '-' && tokens[i].value[tokens[i].value.length() - 1] == '-')
		{
			if (Global::_CSLIDE < 0) Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Defined subtitle outside of slide"));
			else
			{
				std::string tmp = tokens[i].value.substr(1, tokens[i].value.length() - 2);
				Global::_PRESENT->slides[Global::_CSLIDE].subtitle = tmp;
			}
		}

		else if (tokens[i].value[0] == '*')
		{
			if (Global::_CSLIDE < 0) Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Defined subpoint outside of slide"));
			else if (Global::_CPOINT < 0) Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Defined subpoint outside of point"));
			else
			{
				int cutoff = 1;
				if (tokens[i].value[1] == ' ') cutoff = 2;
				Global::_PRESENT->slides[Global::_CSLIDE].points[Global::_CPOINT].subPoints.push_back(tokens[i].value.substr(cutoff, tokens[i].value.length() - 1));
			}
		}

		//Add non-special strings as points to slides
		else if (Global::_CSLIDE < 0) Global::_ERRORS.push_back(ErrorHighlight(ERROR_WARNING, cline, "Defined point outside of slide"));
		else
		{
			Global::_PRESENT->slides[Global::_CSLIDE].points.push_back(Point(tokens[i].value));
			Global::_CPOINT = Global::_CPOINT + 1;
		}

		if(cursorLine == cline)
		{
			if(Global::_CSLIDE != -1) Global::_CSLIDEPREVIEW = Global::_CSLIDE;
			else Global::_FORCEUPDATE = true;
		}
	}

	nDotWarn = 0;
}
