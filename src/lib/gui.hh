#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/wfstream.h>
#include <wx/colordlg.h>
#include <wx/progdlg.h>
#include <wx/aboutdlg.h>

class EditorGUIMain : public wxFrame
{
public:
  EditorGUIMain();

  wxMenuBar* menuBar;
  wxMenu* fileMenu;
  wxMenu* insertMenu;
  wxMenu* helpMenu;

  wxPNGHandler* pngHandler;

  wxBoxSizer* boxSizer;
  wxTextCtrl* textEdit;
  wxStaticBitmap* previewImage;
  wxImage previewImageData;
  
  wxButton* updateBtn;

  wxBoxSizer* preview;
  wxTextCtrl* warnings;

  wxFileDialog* openFileDialog;
  wxDirDialog* saveFileDialog;
  wxFileDialog* exportFileDialog;
  wxFileDialog* openImageDialog;
  wxFileDialog* openFontDialog;
  wxColourDialog* selectColorDialog;

  wxMessageDialog* notSaved;
  wxMessageDialog* error;

  wxTimer* idle;
  wxTimer* cursor;

  long prevCursorPos;

  void update();
  void previewUpdate();
  void timedUpdate(wxTimerEvent &evt);
  void cursorUpdate(wxMouseEvent &evt);
  void timedCursorUpdate(wxTimerEvent &evt);

  void OnClose(wxCloseEvent &evt);

  void save();
  void saveAs();
  void exportPDF();
  void exportPDFAs();
  bool checkSaved();

  void onUpdateClicked(wxCommandEvent& evt);

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

  void onDocsClicked(wxCommandEvent &evt);
  void onAboutClicked(wxCommandEvent &evt);

  void onTextChanged(wxCommandEvent &evt);

  wxDECLARE_EVENT_TABLE();
};

class EditorGUIApp : public wxApp
{
public:
  bool OnInit();
  EditorGUIMain* mainFrame = nullptr;
};