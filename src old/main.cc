/*
BitPresent, an application for creating Presentations in PDF format, based on Markdown styled input
Copyright (C) 2020 BitTim

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "lib/objects.hh"

#include "lib/init.hh"
#include "lib/gui.hh"
#include "lib/cleanup.hh"

int main(int argc, char* argv[])
{	
	Presentation present;

	if (init(present) == false) return -1;
	runGUI(present, argc, argv);
	clean(present);

	return 0;
}