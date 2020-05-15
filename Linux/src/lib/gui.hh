#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

class guiMain : public wxFrame
{
public:
  guiMain();

  wxStaticText* title;
  wxStaticText* inputLabel;
  wxStaticText* outputLabel;
  wxStaticText* statusLabel;

  wxTextCtrl* inputPathBox;
  wxTextCtrl* outputPathBox;
  wxButton* inputBrowseBtn;
  wxButton* outputBrowseBtn;
  wxButton* createBtn;
  wxButton* closeBtn;
  wxGauge* progressBar;
};

class guiApp : public wxApp
{
public:
  virtual bool OnInit();

private:
  guiMain* mainFrame = nullptr;
};

void gprintf(std::string format, ...);