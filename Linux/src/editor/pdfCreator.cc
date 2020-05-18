#include "lib/pdfCreator.hh"

#include <iostream>
#include <vector>

#include "lib/globals.hh"
#include "lib/gui.hh"

HPDF_Error_Handler errorHandler;

int createPDF(std::string outpath)
{
  HPDF_Doc pdf;
	pdf = HPDF_New(errorHandler, NULL);
  if(!pdf)
  {
    Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error creating PDF"));
    return -1;
  }

  HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
  HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_FULL_SCREEN);


  std::vector<HPDF_Page> pages;
  for(int i = 0; i < Global::_PRESENT->slides.size(); i++)
  {
    pages.push_back(HPDF_AddPage(pdf));
    if (pages[i] == nullptr)
    {
      Global::_ERRORS.push_back(ErrorHighlight(ERROR_ERROR, -1, "Error creating Page for PDF"));
      return -1;
    }

    HPDF_Page_SetWidth(pages[i], Global::_WIDTH);
    HPDF_Page_SetHeight(pages[i], Global::_HEIGHT);

    HPDF_Image slide = HPDF_LoadPngImageFromFile2(pdf, (Global::_CACHEPATH.string() + std::to_string(i) + ".png").c_str());
    HPDF_Page_DrawImage(pages[i], slide, 0, 0, HPDF_Image_GetWidth(slide), HPDF_Image_GetHeight(slide));
  }

  HPDF_SaveToFile(pdf, outpath.c_str());
  HPDF_Free(pdf);

  return 0;
}