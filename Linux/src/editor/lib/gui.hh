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

  wxFileDialog* openFileDialog;
  wxFileDialog* saveFileDialog;
  wxFileDialog* exportFileDialog;

  wxMessageDialog* notSaved;

  virtual void OnClose(wxCloseEvent &evt);
  void save();
  void saveAs();

  void onNewClicked(wxCommandEvent &evt);
  void onOpenClicked(wxCommandEvent &evt);
  void onSaveClicked(wxCommandEvent &evt);
  void onSaveAsClicked(wxCommandEvent &evt);
  void onExportClicked(wxCommandEvent &evt);
  void onExportAsClicked(wxCommandEvent &evt);
  void onExitClicked(wxCommandEvent &evt);

  void onBGClicked(wxCommandEvent &evt);
  void onFontClicked(wxCommandEvent &evt);
  void onColorClicked(wxCommandEvent &evt);
  void onTitleClicked(wxCommandEvent &evt);
  void onSlideClicked(wxCommandEvent &evt);
  void onSubtitleClicked(wxCommandEvent &evt);
  void onImageClicked(wxCommandEvent &evt);
  void onSubpointClicked(wxCommandEvent &evt);

  void onTextChanged(wxCommandEvent &evt);

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

void clearTerminal();
void resetProgress();