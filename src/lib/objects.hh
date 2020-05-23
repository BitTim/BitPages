#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <vector>

struct Point
{
	Point(std::string t) { text = t; }

	std::string text;
	std::vector<std::string> subPoints;

	void clean()
	{
		text.clear();
		
		for(int i = 0; i < subPoints.size(); i++) subPoints[i].clear();
		subPoints.clear();
	}
};

struct Slide
{
	bool titleSlide = false;
	std::string title;
	std::string subtitle;
	std::string image = "none";
	std::vector<Point> points;

	void clean()
	{
		titleSlide = false;
		title.clear();
		subtitle.clear();
		image = "none";

		for(int i = 0; i < points.size(); i++) points[i].clean();
		points.clear();
	}
};

struct Presentation
{
	Presentation();

	std::vector<Slide> slides;
	std::string background;
	SDL_Color* textcolor;
	std::map<std::string, TTF_Font*> font;

	void clean();
};