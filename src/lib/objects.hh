#pragma once

#include <string>
#include <vector>

struct Slide
{
	bool titleSlide = false;
	std::string title;
	std::string subtitle;
	std::string image = "none";
	std::vector<std::string> points;
};

struct Presentation
{
	std::vector<Slide> slides;
};