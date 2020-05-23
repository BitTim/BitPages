#pragma once

#include <wx/wxprec.h>
    #ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/wfstream.h>
#include <wx/stc/stc.h>
#include <wx/colordlg.h>
#include <wx/progdlg.h>
#include <wx/aboutdlg.h>

#include "objects.hh"

void runGUI(Presentation &present, int argc, char* argv[]);

class GUIMain : public wxFrame
{
public:
  GUIMain();
  wxTimer* getCursorTimer();
  long getCursorPos();

private:
    void initMenuBar();
    wxMenu* createMenu(std::vector<int> ids, std::vector<std::string> entires, std::vector<int> seperators);
    wxMenuBar* createMenuBar(std::vector<wxMenu*> menus, std::vector<std::string> entries);

    void initLayout();
    wxBoxSizer* createMainLayout();
    wxBoxSizer* createRightLayout();

    void initStyles();
    void initDialogs();

    void clearPreview();

    wxStyledTextCtrl* textEdit;
    wxTextCtrl* warnings;
    wxStaticBitmap* preview;

    wxPNGHandler* pngHandler;
    wxImage previewImage;
  
    wxFileDialog* openFileDialog;
    wxDirDialog* saveFileDialog;
    wxFileDialog* exportFileDialog;
    wxFileDialog* openImageDialog;
    wxFileDialog* openFontDialog;
    wxColourDialog* selectColorDialog;

    wxMessageDialog* notSaved;

    wxTimer* idle;
    wxTimer* cursor;

    wxDECLARE_EVENT_TABLE();
};

class GUIApp : public wxApp
{
public:
    GUIMain* mainFrame = nullptr;
};