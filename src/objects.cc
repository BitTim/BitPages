#include "lib/objects.hh"
#include "lib/globals.hh"

Presentation::Presentation()
{
  background = Global::_DEFAULTBACKGROUND;
  textcolor = Global::_DEFAULTTEXTCOLOR;
  font = Global::_DEFAULTFONT;
}

void Presentation::clean()
{
  for(int i = 0; i < slides.size(); i++) slides[i].clean();
	slides.clear();
	background = Global::_DEFAULTBACKGROUND;
	textcolor = Global::_DEFAULTTEXTCOLOR;
	font = Global::_DEFAULTFONT;
}