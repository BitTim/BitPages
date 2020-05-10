#pragma once

#include <string>
#include <vector>

struct Point
{
	Point(std::string t) { text = t; }

	std::string text;
	std::vector<std::string> subPoints;
};

struct Slide
{
	bool titleSlide = false;
	std::string title;
	std::string subtitle;
	std::string image = "none";
	std::vector<Point> points;
};

struct Presentation
{
	std::vector<Slide> slides;
};