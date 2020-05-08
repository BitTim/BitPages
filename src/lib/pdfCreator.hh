#pragma once

#include "hpdf.h"
#include <iostream>
#include <vector>
#include "globals.hh"

void errorHandler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
  printf("[ERROR]: Error with pdf: %d\n", error_no);
}

int createPDF()
{
  HPDF_Doc pdf;
	pdf = HPDF_New(errorHandler, NULL);
  if(!pdf)
  {
    printf("[ERROR]: Could not create pdf\n");
    return -1;
  }

  HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
  HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_FULL_SCREEN);

  std::vector<HPDF_Page> pages;
  for(int i = 0; i < Global::_PRESENT->slides.size(); i++)
  {
    pages.push_back(HPDF_AddPage(pdf));
    HPDF_Page_SetWidth(pages[i], Global::_WIDTH);
    HPDF_Page_SetHeight(pages[i], Global::_HEIGHT);

    HPDF_Image slide = HPDF_LoadPngImageFromFile2(pdf, (".cache/" + std::to_string(i) + ".png").c_str());
    HPDF_Page_DrawImage(pages[i], slide, 0, 0, HPDF_Image_GetWidth(slide), HPDF_Image_GetHeight(slide));
  }

  HPDF_SaveToFile(pdf, Global::_OUTPATH.c_str());
  HPDF_Free(pdf);

  return 0;
}