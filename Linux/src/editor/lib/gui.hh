#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/wfstream.h>

class EditorGUIMain : public wxFrame
{
public:
  EditorGUIMain();

  wxMenuBar* menuBar;
  wxMenu* fileMenu;
  wxMenu* insertMenu;

  wxBoxSizer* boxSizer;
  wxTextCtrl* textEdit;
  wxStaticBitmap* preview;

  wxDECLARE_EVENT_TABLE();
};

class EditorGUIApp : public wxApp
{
public:
  virtual bool OnInit();
  EditorGUIMain* mainFrame = nullptr;
};

void gprintf(std::string format, ...);
void changeStatus(std::string status);
void progress();