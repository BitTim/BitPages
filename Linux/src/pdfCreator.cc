#include "lib/pdfCreator.hh"

#include <iostream>
#include <vector>
#include "lib/globals.hh"
#include "lib/gui.hh"

void errorHandler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
  gprintf("[ERROR]: Error with pdf: %04x, details: %s\n", error_no, strerror(detail_no));
}

int createPDF(std::string outpath)
{
  HPDF_Doc pdf;
	pdf = HPDF_New(errorHandler, NULL);
  if(!pdf)
  {
    gprintf("[ERROR]: Could not create pdf\n");
    return -1;
  }

  if(Global::useGUI) progress();

  HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
  HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_FULL_SCREEN);

  if(Global::useGUI) progress();

  std::vector<HPDF_Page> pages;
  for(int i = 0; i < Global::_PRESENT[Global::_CPRESENT]->slides.size(); i++)
  {
    pages.push_back(HPDF_AddPage(pdf));
    if (pages[i] == nullptr)
    {
      gprintf("[ERROR]: Failed to create page for PDF\n");
      return -1;
    }

    HPDF_Page_SetWidth(pages[i], Global::_WIDTH);
    HPDF_Page_SetHeight(pages[i], Global::_HEIGHT);

    HPDF_Image slide = HPDF_LoadPngImageFromFile2(pdf, (".cache/" + std::to_string(i) + ".png").c_str());
    HPDF_Page_DrawImage(pages[i], slide, 0, 0, HPDF_Image_GetWidth(slide), HPDF_Image_GetHeight(slide));
  }

  if(Global::useGUI) progress();

  HPDF_SaveToFile(pdf, outpath.c_str());
  HPDF_Free(pdf);

  if(Global::useGUI) progress();
  return 0;
}