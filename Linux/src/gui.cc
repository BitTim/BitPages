#include "lib/gui.hh"
#include "lib/globals.hh"

guiMain::guiMain() : wxFrame(nullptr, wxID_ANY, "", wxDefaultPosition, wxSize(640, 480))
{
  title = new wxStaticText(this, wxID_ANY, Global::_VERSIONSTRING, wxPoint(20, 10), wxSize(600, 60));
  inputLabel = new wxStaticText(this, wxID_ANY, "Input (.txt):", wxPoint(20, 80));
  outputLabel = new wxStaticText(this, wxID_ANY, "Output (.pdf):", wxPoint(20, 110));
  statusLabel = new wxStaticText(this, wxID_ANY, "Ready", wxPoint(20, 140));

  inputPathBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(160, 80), wxSize(350, 20));
  outputPathBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(160, 110), wxSize(350, 20));
  inputBrowseBtn = new wxButton(this, wxID_ANY, "Browse", wxPoint(520, 80), wxSize(100, 20));
  outputBrowseBtn = new wxButton(this, wxID_ANY, "Browse", wxPoint(520, 110), wxSize(100, 20));
  createBtn = new wxButton(this, wxID_ANY, "Create", wxPoint(520, 140), wxSize(100, 20));
  closeBtn = new wxButton(this, wxID_ANY, "Close", wxPoint(410, 140), wxSize(100, 20));
  progressBar = new wxGauge(this, wxID_ANY, ' ', wxPoint(20, 170), wxSize(600, 20));

  wxFont font = title->GetFont();
  font.SetPointSize(30);
  font.SetWeight(wxFONTWEIGHT_BOLD);
  title->SetFont(font);
  
  font = statusLabel->GetFont();
  font.SetStyle(wxFONTSTYLE_ITALIC);
  statusLabel->SetFont(font);
}

bool guiApp::OnInit()
{
  mainFrame = new guiMain();
  mainFrame->SetTitle(Global::_VERSIONSTRING);
  mainFrame->Show();

  return true;
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
    //gtk_text_buffer_get_end_iter(buffer, &bufferEnd);
    //gtk_text_buffer_insert(buffer, &bufferEnd, tmp.c_str() , -1);

    //while (gtk_events_pending ()) gtk_main_iteration ();
  }

  va_end(args);
}