#pragma once

#include "objects.hh"

void update();
void previewUpdate(Presentation& present, int slide);
void save(bool saveAs = false);
void exportPDF(bool exportAs = false);