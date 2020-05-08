#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

#include "objects.hh"

std::vector<std::string> tokenize(std::string path)
{
	std::vector<std::string> tokens;

	std::ifstream file(path.c_str());
	if (file.fail()) return tokens;

	std::string line;
	std::string buffer;

	while(std::getline(file, line))
	{
		if(line == "") continue;

		if (line[0] == '.')
		{
			int index = line.find(" ");
			if(index == std::string::npos)
			{
				printf("[WARNING]: Invalid '.background' statement. Ignoring\n");
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
	for(int i = 0; i < tokens.size(); i++)
	{
		//Check for commands
		if(tokens[i] == ".background")
		{
			i += 1;
			if(tokens[i][0] == '<' || tokens[i][0] == '[' || tokens[i][0] == '-') printf("[WARNING]: Using instruction(\"%s\") as argument for '.background'\n", tokens[i].c_str());
			Global::_PRESENT->background = tokens[i];
			continue;
		}
		
		if(tokens[i] == ".image")
		{
			i += 1;
			if(tokens[i][0] == '<' || tokens[i][0] == '[' || tokens[i][0] == '-') printf("[WARNING]: Using instruction(\"%s\") as argument for '.image'\n", tokens[i].c_str());
			if(Global::_CSLIDE < 0) printf("[WARNING]: '.image' used outside of a slide. Ignoring...\n");
			else Global::_PRESENT->slides[Global::_CSLIDE].image = tokens[i];
			continue;
		}

		if(tokens[i] == ".font")
		{
			i += 1;
			if(tokens[i][0] == '<' || tokens[i][0] == '[' || tokens[i][0] == '-') printf("[WARNING]: Using instruction(\"%s\") as argument for '.image'\n", tokens[i].c_str());
			Global::_FONT = TTF_OpenFont(tokens[i].c_str(), 16);
			if(Global::_FONT == NULL)
			{
				printf("[ERROR]: Error loading font. Reverting to default\n");
				Global::_FONT = Global::_DEFAULTFONT;
			}
			continue;
		}

		//Check for other defining Tokens
		if(tokens[i][0] == '<' && tokens[i][tokens[i].length() - 1] == '>')
		{
			Global::_PRESENT->slides.push_back(Slide());
			Global::_CSLIDE = Global::_CSLIDE + 1;

			Global::_PRESENT->slides[Global::_CSLIDE].titleSlide = true;
			Global::_PRESENT->slides[Global::_CSLIDE].title = tokens[i].substr(1, tokens[i].length() - 2);
			continue;
		}

		if(tokens[i][0] == '[' && tokens[i][tokens[i].length() - 1] == ']')
		{
			Global::_PRESENT->slides.push_back(Slide());
			Global::_CSLIDE = Global::_CSLIDE + 1;

			Global::_PRESENT->slides[Global::_CSLIDE].title = tokens[i].substr(1, tokens[i].length() - 2);
			continue;
		}

		if(tokens[i][0] == '-' && tokens[i][tokens[i].length() - 1] == '-')
		{
			if(Global::_CSLIDE < 0) printf("[WARNING]: Defined subtitle outside of a slide. Ignoring...\n");
			else Global::_PRESENT->slides[Global::_CSLIDE].subtitle = tokens[i].substr(1, tokens[i].length() - 2);
			continue;
		}

		//Add non-special strings as points to slides
		if(Global::_CSLIDE < 0) printf("[WARNING]: Defined point outside of a slide. Ignoring...\n");
		else Global::_PRESENT->slides[Global::_CSLIDE].points.push_back(tokens[i]);
	}
	printf("[OK]: Finished Parsing\n");
}