#include "lib/gui.hh"
#include "lib/globals.hh"
#include "lib/presentCreator.hh"

wxBEGIN_EVENT_TABLE(guiMain, wxFrame)
  EVT_BUTTON(10001, guiMain::OnInBtnClicked)
  EVT_BUTTON(10002, guiMain::OnOutBtnClicked)
  EVT_BUTTON(10003, guiMain::OnCreateBtnClicked)
  EVT_BUTTON(10004, guiMain::OnCloseBtnClicked)
wxEND_EVENT_TABLE()

guiMain::guiMain() : wxFrame(nullptr, wxID_ANY, "", wxDefaultPosition, wxSize(640, 480))
{
  title = new wxStaticText(this, wxID_ANY, Global::_VERSIONSTRING, wxPoint(20, 10), wxSize(600, 60));
  inputLabel = new wxStaticText(this, wxID_ANY, "Input (.txt):", wxPoint(20, 80));
  outputLabel = new wxStaticText(this, wxID_ANY, "Output (.pdf):", wxPoint(20, 110));
  statusLabel = new wxStaticText(this, wxID_ANY, "Ready", wxPoint(20, 140));

  pngHandler = new wxPNGHandler();
  wxImage tmp;

  wxFileInputStream bpInStream("dat/BPLogo.png");
  pngHandler->LoadFile(&tmp, bpInStream);
  bplogo = new wxStaticBitmap(this, wxID_ANY, wxBitmap(tmp), wxPoint(510, 10), wxSize(50, 50));

  wxFileInputStream btInStream("dat/BTLogo.png");
  pngHandler->LoadFile(&tmp, btInStream);
  btlogo = new wxStaticBitmap(this, wxID_ANY, wxBitmap(tmp), wxPoint(570, 10), wxSize(50, 50));

  inputPathBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(160, 80), wxSize(350, 20));
  outputPathBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(160, 110), wxSize(350, 20));
  inputBrowseBtn = new wxButton(this, 10001, _("Browse"), wxPoint(520, 80), wxSize(100, 20));
  outputBrowseBtn = new wxButton(this, 10002, _("Browse"), wxPoint(520, 110), wxSize(100, 20));
  createBtn = new wxButton(this, 10003, _("Create"), wxPoint(520, 140), wxSize(100, 20));
  closeBtn = new wxButton(this, 10004, _("Close"), wxPoint(410, 140), wxSize(100, 20));
  progressBar = new wxGauge(this, wxID_ANY, ' ', wxPoint(20, 170), wxSize(600, 20));
  terminal = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(20, 200), wxSize(600, 260), (wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxTE_RICH2));

  wxFont font = title->GetFont();
  font.SetPointSize(30);
  font.SetWeight(wxFONTWEIGHT_BOLD);
  title->SetFont(font);
  
  font = statusLabel->GetFont();
  font.SetStyle(wxFONTSTYLE_ITALIC);
  statusLabel->SetFont(font);

  terminal->SetBackgroundColour(wxColour(0x33, 0x33, 0x33));
  terminal->SetDefaultStyle(wxTextAttr(wxColour(0xDC, 0xDC, 0xDC)));

  font = terminal->GetFont();
  font.SetFamily(wxFONTFAMILY_TELETYPE);
  terminal->SetFont(font);
  terminal->AppendText(Global::_VERSIONSTRING + " - Ready\n");

  openFileDialog = new wxFileDialog(this, _("Select an input file"), wxEmptyString, wxEmptyString, "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  saveFileDialog = new wxFileDialog(this, _("Select an output file"), wxEmptyString, "Presentation.pdf", "PDF Documents (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
}

bool guiApp::OnInit()
{
  mainFrame = new guiMain();
  mainFrame->SetMinSize(mainFrame->GetSize());
  mainFrame->SetMaxSize(mainFrame->GetSize());
  mainFrame->SetTitle(Global::_VERSIONSTRING);
  mainFrame->Show();

  return true;
}

void guiMain::OnInBtnClicked(wxCommandEvent &evt)
{
  if(openFileDialog->ShowModal() != wxID_CANCEL) inputPathBox->SetValue(openFileDialog->GetPath());
  evt.Skip();
}

void guiMain::OnOutBtnClicked(wxCommandEvent &evt)
{
  if(saveFileDialog->ShowModal() != wxID_CANCEL) outputPathBox->SetValue(saveFileDialog->GetPath());
  evt.Skip();
}

void guiMain::OnCloseBtnClicked(wxCommandEvent &evt)
{
  Close(TRUE);
  evt.Skip();
}

void guiMain::OnCreateBtnClicked(wxCommandEvent &evt)
{
  createPresent((std::string) inputPathBox->GetValue(), (std::string) outputPathBox->GetValue());
  evt.Skip();
}

void gprintf(std::string format, ...)
{
  va_list args;
  va_start (args, format);

  char buf[1024];
  std::string tmp;

  vsprintf(buf, format.c_str(), args);
  tmp = std::string(buf);

  printf("%s", tmp.c_str());
  if(Global::useGUI)
  {
    Global::gApp->mainFrame->terminal->AppendText(tmp);
    Global::gApp->mainFrame->Refresh();
    Global::gApp->mainFrame->Update();
  }

  va_end(args);
}