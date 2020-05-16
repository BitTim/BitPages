#include "lib/gui.hh"
#include "lib/globals.hh"

wxBEGIN_EVENT_TABLE(EditorGUIMain, wxFrame)

wxEND_EVENT_TABLE()

EditorGUIMain::EditorGUIMain() : wxFrame(nullptr, wxID_ANY, "")
{
  fileMenu = new wxMenu();
  fileMenu->Append(wxID_NEW);
  fileMenu->Append(wxID_OPEN);
  fileMenu->Append(wxID_SAVE);
  fileMenu->Append(wxID_SAVEAS);
  fileMenu->Append(10001, _("&Export\tCtrl+E"));
  fileMenu->Append(10002, _("E&xport As..."));
  fileMenu->Append(wxID_EXIT);

  insertMenu = new wxMenu();
  insertMenu->Append(10003, _("&Background Image"));
  insertMenu->Append(10004, _("&Font"));
  insertMenu->Append(10005, _("Text &Color"));
  insertMenu->Append(10006, _("&Title Slide"));
  insertMenu->Append(10007, _("&Slide"));
  insertMenu->Append(10008, _("Su&btitle"));
  insertMenu->Append(10009, _("&Image"));
  insertMenu->Append(10010, _("Sub&point"));

  menuBar = new wxMenuBar();
  menuBar->Append(fileMenu, _("&File"));
  menuBar->Append(insertMenu, _("&Insert"));
  SetMenuBar(menuBar);

  boxSizer = new wxBoxSizer(wxHORIZONTAL);
  textEdit = new wxTextCtrl(this , 10011, wxEmptyString, wxDefaultPosition, wxDefaultSize, (wxTE_MULTILINE | wxTE_RICH2));
  preview = new wxStaticBitmap(this, wxID_ANY, wxBitmap());

  boxSizer->Add(textEdit);
  boxSizer->Add(preview);
}

bool EditorGUIApp::OnInit()
{
  mainFrame = new EditorGUIMain();
  mainFrame->SetTitle(Global::_VERSIONSTRING);
  mainFrame->Show();

  return true;
}

void gprintf(std::string format, ...) {}
void changeStatus(std::string status) {}
void progress() {}