#include "lib/guiEvents.hh"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "lib/gui.hh"

wxBEGIN_EVENT_TABLE(GUIMain, wxFrame)
	EVT_CLOSE(EditorGUIMain::OnClose)
	EVT_TIMER(10014, EditorGUIMain::timedUpdate)
	EVT_TIMER(10015, EditorGUIMain::timedCursorUpdate)
	
	EVT_MENU(wxID_NEW, EditorGUIMain::onNewClicked)
	EVT_MENU(wxID_OPEN, EditorGUIMain::onOpenClicked)
	EVT_MENU(wxID_SAVE, EditorGUIMain::onSaveClicked)
	EVT_MENU(wxID_SAVEAS, EditorGUIMain::onSaveAsClicked)
	EVT_MENU(10001, EditorGUIMain::onExportClicked)
	EVT_MENU(10002, EditorGUIMain::onExportAsClicked)
	EVT_MENU(wxID_EXIT, EditorGUIMain::onExitClicked)

	EVT_MENU(10003, EditorGUIMain::onBGClicked)
	EVT_MENU(10004, EditorGUIMain::onFontClicked)
	EVT_MENU(10005, EditorGUIMain::onColorClicked)
	EVT_MENU(10006, EditorGUIMain::onTitleClicked)
	EVT_MENU(10007, EditorGUIMain::onSlideClicked)
	EVT_MENU(10008, EditorGUIMain::onImageClicked)
	EVT_MENU(10009, EditorGUIMain::onSubtitleClicked)
	EVT_MENU(10010, EditorGUIMain::onSubpointClicked)

	EVT_BUTTON(10018, EditorGUIMain::onUpdateClicked)

	EVT_MENU(10016, EditorGUIMain::onDocsClicked)
	EVT_MENU(10017, EditorGUIMain::onAboutClicked)

	EVT_TEXT(10011, EditorGUIMain::onTextChanged)
wxEND_EVENT_TABLE()

bool OnInit()
{
	guiApp->mainFrame = new GUIMain();
	guiApp->mainFrame->SetTitle(Global::_VERSIONSTRING);
	guiApp->mainFrame->Show();
	guiApp->mainFrame->Maximize();

	prevCursorPos = guiApp->mainFrame->getCursorPos();
	guiApp->mainFrame->SetBackgroundColour(wxColor(0xDC, 0xDC, 0xDC));
	guiApp->mainFrame->getCursorTimer()->Start(1);

	guiApp->mainFrame->Refresh();
	guiApp->mainFrame->Update();

	return true;
}

void OnClose(wxCloseEvent& evt);

void onTextChanged(wxCommandEvent& evt);
void timedUpdate(wxTimerEvent& evt);
void cursorUpdate(wxMouseEvent& evt);
void timedCursorUpdate(wxTimerEvent& evt);

void onUpdateClicked(wxCommandEvent& evt);

void onNewClicked(wxCommandEvent& evt);
void onOpenClicked(wxCommandEvent& evt);
void onSaveClicked(wxCommandEvent& evt);
void onSaveAsClicked(wxCommandEvent& evt);
void onExportClicked(wxCommandEvent& evt);
void onExportAsClicked(wxCommandEvent& evt);
void onExitClicked(wxCommandEvent& evt);

void onBGClicked(wxCommandEvent& evt);
void onFontClicked(wxCommandEvent& evt);
void onColorClicked(wxCommandEvent& evt);
void onTitleClicked(wxCommandEvent& evt);
void onSlideClicked(wxCommandEvent& evt);
void onSubtitleClicked(wxCommandEvent& evt);
void onImageClicked(wxCommandEvent& evt);
void onSubpointClicked(wxCommandEvent& evt);

void onDocsClicked(wxCommandEvent& evt);
void onAboutClicked(wxCommandEvent& evt);