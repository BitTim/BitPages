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

  wxPNGHandler* pngHandler;

  wxBoxSizer* boxSizer;
  wxTextCtrl* textEdit;
  wxStaticBitmap* previewImage;
  wxImage previewImageData;
  
  wxBoxSizer* preview;
  wxTextCtrl* warnings;

  wxFileDialog* openFileDialog;
  wxFileDialog* saveFileDialog;
  wxFileDialog* exportFileDialog;

  wxMessageDialog* notSaved;
  wxMessageDialog* error;

  wxTimer* idle;
  wxTimer* cursor;

  long prevCursorPos;

  void loadPreview();
  void update();
  void previewUpdate();
  void timedUpdate(wxTimerEvent &evt);
  void cursorUpdate(wxMouseEvent &evt);
  void timedCursorUpdate(wxTimerEvent &evt);

  void OnClose(wxCloseEvent &evt);
  void resizing(wxSizeEvent &evt);

  void save();
  void saveAs();
  bool checkSaved();

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
  bool OnInit();
  EditorGUIMain* mainFrame = nullptr;
};

void gprintf(std::string format, ...);
void changeStatus(std::string status);
void progress();

void clearTerminal();
void resetProgress();