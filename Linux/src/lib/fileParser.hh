#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

#include "objects.hh"

int nDotWarn = 0;

std::vector<std::string> tokenize(std::string path)
{
	std::vector<std::string> tokens;

	std::ifstream file(path.c_str());
	if (file.fail()) return tokens;

	std::string line;
	std::string buffer;
	int cline = 0;

	while (std::getline(file, line))
	{
		cline += 1;
		if (line[0] == '.')
		{
			int index = line.find(" ");
			if (index == std::string::npos)
			{
				printf("[WARNING](Line %d): Invalid '.' command statement. Ignoring\n", cline);
				nDotWarn++;
				continue;
			}
			else
			{
				tokens.push_back(line.substr(0, index));
				tokens.push_back(line.substr(index + 1, line.length() - 1));
			}
		}
		else
		{
			tokens.push_back(line);
		}
	}

	file.close();
	return tokens;
}

void parse(std::vector<std::string> tokens)
{
	int cline = nDotWarn;

	for (int i = 0; i < tokens.size(); i++)
	{
		cline += 1;

		//Skip empty tokens
		if (tokens[i] == "") continue;

		//Check for commands
		if (tokens[i] == ".background")
		{
			i += 1;
			if (tokens[i][0] == '<' || tokens[i][0] == '[' || tokens[i][0] == '-') printf("[WARNING](Line %d): Using instruction(\"%s\") as argument for '.background'\n", cline, tokens[i].c_str());
			Global::_BACKGROUND = tokens[i];
			continue;
		}

		if (tokens[i] == ".image")
		{
			i += 1;
			if (tokens[i][0] == '<' || tokens[i][0] == '[' || tokens[i][0] == '-') printf("[WARNING](Line %d): Using instruction(\"%s\") as argument for '.image'\n", cline, tokens[i].c_str());
			if (Global::_CSLIDE < 0) printf("[WARNING]: '.image' used outside of a slide. Ignoring...\n");
			else Global::_PRESENT->slides[Global::_CSLIDE].image = tokens[i];
			continue;
		}

		if (tokens[i] == ".font")
		{
			i += 1;
			if (tokens[i][0] == '<' || tokens[i][0] == '[' || tokens[i][0] == '-') printf("[WARNING](Line %d): Using instruction(\"%s\") as argument for '.image'\n", cline, tokens[i].c_str());
			Global::_FONT = { {"title", TTF_OpenFont(tokens[i].c_str(), 68)}, {"subtitle", TTF_OpenFont(tokens[i].c_str(), 50)}, {"normal", TTF_OpenFont(tokens[i].c_str(), 34)} };
			if (Global::_FONT["title"] == NULL || Global::_FONT["subtitle"] == NULL || Global::_FONT["normal"] == NULL)
			{
				printf("[ERROR](Line %d): Error loading font. Reverting to default\n", cline);
				Global::_FONT = Global::_DEFAULTFONT;
			}
			continue;
		}

		if (tokens[i] == ".color")
		{
			i += 1;
			if (tokens[i] == "default") Global::_TEXTCOLOR = Global::_DEFAULTTEXTCOLOR;
			else
			{
				uint8_t r = std::stoi(tokens[i].substr(0, 2), nullptr, 16);
				uint8_t g = std::stoi(tokens[i].substr(2, 2), nullptr, 16);
				uint8_t b = std::stoi(tokens[i].substr(4, 2), nullptr, 16);

				Global::_TEXTCOLOR = new SDL_Color{ r, g, b };
			}
			continue;
		}

		//Check for other defining Tokens
		if (tokens[i][0] == '<' && tokens[i][tokens[i].length() - 1] == '>')
		{
			Global::_PRESENT->slides.push_back(Slide());
			Global::_CSLIDE = Global::_CSLIDE + 1;
			Global::_CPOINT = -1;

			Global::_PRESENT->slides[Global::_CSLIDE].titleSlide = true;
			Global::_PRESENT->slides[Global::_CSLIDE].title = tokens[i].substr(1, tokens[i].length() - 2);
			continue;
		}

		if (tokens[i][0] == '[' && tokens[i][tokens[i].length() - 1] == ']')
		{
			Global::_PRESENT->slides.push_back(Slide());
			Global::_CSLIDE = Global::_CSLIDE + 1;
			Global::_CPOINT = -1;

			Global::_PRESENT->slides[Global::_CSLIDE].title = tokens[i].substr(1, tokens[i].length() - 2);
			continue;
		}

		if (tokens[i][0] == '-' && tokens[i][tokens[i].length() - 1] == '-')
		{
			if (Global::_CSLIDE < 0) printf("[WARNING](Line %d): Defined subtitle outside of a slide. Ignoring...\n", cline);
			else Global::_PRESENT->slides[Global::_CSLIDE].subtitle = tokens[i].substr(1, tokens[i].length() - 2);
			continue;
		}

		if (tokens[i][0] == '*')
		{
			if (Global::_CSLIDE < 0) printf("[WARNING](Line %d): Defined subpoint outside of a slide. Ignoring...\n", cline);
			else if (Global::_CPOINT < 0) printf("[WARNING](Line %d): Defined subpoint outside of a point. Ignoring...\n", cline);
			else
			{
				int cutoff = 1;
				if (tokens[i][1] == ' ') cutoff = 2;
				Global::_PRESENT->slides[Global::_CSLIDE].points[Global::_CPOINT].subPoints.push_back(tokens[i].substr(cutoff, tokens[i].length() - 1));
			}
			continue;
		}

		//Add non-special strings as points to slides
		if (Global::_CSLIDE < 0) printf("[WARNING](Line %d): Defined point outside of a slide. Ignoring...\n", cline);
		else
		{
			Global::_PRESENT->slides[Global::_CSLIDE].points.push_back(Point(tokens[i]));
			Global::_CPOINT = Global::_CPOINT + 1;
		}
	}
	printf("[OK]: Finished Parsing\n");
}
