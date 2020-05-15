#include "lib/gui.hh"
#include "lib/globals.hh"

wxIMPLEMENT_APP(guiApp);

guiMain::guiMain() : wxFrame(nullptr, wxID_ANY, "", wxDefaultPosition, wxSize(640, 480))
{
  inputPathBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(20, 20), wxSize(400, 20));
  outputPathBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(20, 40), wxSize(400, 20));
  inputBrowseBtn = new wxButton(this, wxID_ANY, wxEmptyString, wxPoint(20, 60), wxSize(400, 20));
  outputBrowseBtn = new wxButton(this, wxID_ANY, wxEmptyString, wxPoint(20, 80), wxSize(400, 20));
  createBtn = new wxButton(this, wxID_ANY, wxEmptyString, wxPoint(20, 100), wxSize(400, 20));
  closeBtn = new wxButton(this, wxID_ANY, wxEmptyString, wxPoint(20, 120), wxSize(400, 20));
  progressBar = new wxGauge(this, wxID_ANY, ' ', wxPoint(20, 140), wxSize(400, 20));
}

bool guiApp::OnInit()
{
  mainFrame = new guiMain();
  mainFrame->SetTitle(Global::_VERSIONSTRING);
  mainFrame->Show();

  return true;
}