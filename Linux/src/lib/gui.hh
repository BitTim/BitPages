#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/wfstream.h>

class guiMain : public wxFrame
{
public:
  guiMain();

  wxStaticText* title;
  wxStaticText* inputLabel;
  wxStaticText* outputLabel;
  wxStaticText* statusLabel;

  wxPNGHandler* pngHandler;
  wxStaticBitmap* bplogo;
  wxStaticBitmap* btlogo;

  wxTextCtrl* inputPathBox;
  wxTextCtrl* outputPathBox;
  wxButton* inputBrowseBtn;
  wxButton* outputBrowseBtn;
  wxButton* createBtn;
  wxButton* closeBtn;
  wxGauge* progressBar;
  wxTextCtrl* terminal;

  wxFileDialog* openFileDialog;
  wxFileDialog* saveFileDialog;

  void OnInBtnClicked(wxCommandEvent &evt);
  void OnOutBtnClicked(wxCommandEvent &evt);
  void OnCloseBtnClicked(wxCommandEvent &evt);
  void OnCreateBtnClicked(wxCommandEvent &evt);

  wxDECLARE_EVENT_TABLE();
};

class guiApp : public wxApp
{
public:
  virtual bool OnInit();
  guiMain* mainFrame = nullptr;
};

void gprintf(std::string format, ...);