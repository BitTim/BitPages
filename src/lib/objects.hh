#pragma once

#include <string>
#include <vector>

struct Slide
{
	bool titleSlide = false;
	std::string title;
	std::string subtitle;
	std::string image;
	std::vector<std::string> points;
};

struct Presentation
{
	std::string background;
	std::vector<Slide> slides;
};