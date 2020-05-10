#pragma once
#include "hpdf.h"
#include <string>

extern void errorHandler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data);
extern int createPDF(std::string outpath);