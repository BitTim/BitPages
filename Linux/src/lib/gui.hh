#pragma once
#include <wx/wx.h>

class guiMain : public wxFrame
{
public:
  guiMain();

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