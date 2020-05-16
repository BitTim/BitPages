#include "lib/gui.hh"
#include "lib/globals.hh"

wxBEGIN_EVENT_TABLE(EditorGUIMain, wxFrame)
  EVT_CLOSE(EditorGUIMain::OnClose)

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

  EVT_TEXT(10011, EditorGUIMain::onTextChanged)
wxEND_EVENT_TABLE()

EditorGUIMain::EditorGUIMain() : wxFrame(nullptr, wxID_ANY, "")
{
  fileMenu = new wxMenu();
  fileMenu->Append(wxID_NEW);
  fileMenu->Append(wxID_OPEN);
  fileMenu->Append(wxID_SAVE);
  fileMenu->Append(wxID_SAVEAS);
  fileMenu->AppendSeparator();
  fileMenu->Append(10001, _("&Export\tCtrl+E"));
  fileMenu->Append(10002, _("E&xport As..."));
  fileMenu->AppendSeparator();
  fileMenu->Append(wxID_EXIT);

  insertMenu = new wxMenu();
  insertMenu->Append(10003, _("&Background Image"));
  insertMenu->Append(10004, _("&Font"));
  insertMenu->Append(10005, _("Text &Color"));
  insertMenu->AppendSeparator();
  insertMenu->Append(10006, _("&Title Slide"));
  insertMenu->Append(10007, _("&Slide"));
  insertMenu->AppendSeparator();
  insertMenu->Append(10008, _("&Image"));
  insertMenu->Append(10009, _("Su&btitle"));
  insertMenu->Append(10010, _("Sub&point"));

  menuBar = new wxMenuBar();
  menuBar->Append(fileMenu, _("&File"));
  menuBar->Append(insertMenu, _("&Insert"));
  SetMenuBar(menuBar);

  boxSizer = new wxBoxSizer(wxHORIZONTAL);
  textEdit = new wxTextCtrl(this , 10011, wxEmptyString, wxDefaultPosition, wxDefaultSize, (wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL));
  preview = new wxStaticBitmap(this, wxID_ANY, wxBitmap());

  boxSizer->Add(textEdit, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 10);
  boxSizer->Add(preview, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 10);
  SetSizerAndFit(boxSizer);

  wxFont font = textEdit->GetFont();
  font.SetFamily(wxFONTFAMILY_TELETYPE);
  textEdit->SetFont(font);

  openFileDialog = new wxFileDialog(this, _("Open file"), wxEmptyString, wxEmptyString, "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  saveFileDialog = new wxFileDialog(this, _("Save file"), wxEmptyString, "Presentation.txt", "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  exportFileDialog = new wxFileDialog(this, _("Select an input file"), wxEmptyString, "Presentation.pdf", "PDF Documents (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  notSaved = new wxMessageDialog(this, "The file contains unsaved changes. Save changes?", "Unsaved file", wxYES_NO | wxCANCEL | wxNO_DEFAULT);
}

bool EditorGUIApp::OnInit()
{
  mainFrame = new EditorGUIMain();
  mainFrame->SetMinSize(wxSize(640, 480));
  mainFrame->SetTitle(Global::_VERSIONSTRING);
  mainFrame->Show();

  return true;
}

void EditorGUIMain::OnClose(wxCloseEvent& evt)
{
  if (evt.CanVeto() && !Global::_SAVED)
  {
    int decision = notSaved->ShowModal();
    if(decision == wxID_NO) Close(TRUE);
    else if(decision == wxID_YES) save();
    else evt.Veto();
  }
  else Destroy();
}

void EditorGUIMain::save()
{
  if(Global::_SAVEPATH == "") saveAs();
  else
  {
    textEdit->SaveFile(Global::_SAVEPATH);
    Global::_SAVED = true;
  }
}

void EditorGUIMain::saveAs()
{
  if(saveFileDialog->ShowModal() != wxID_CANCEL)
  {
    Global::_SAVEPATH = saveFileDialog->GetPath();
    textEdit->SaveFile(Global::_SAVEPATH);
    Global::_SAVED = true;
  }
}

void EditorGUIMain::onNewClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onOpenClicked(wxCommandEvent &evt)
{
  if(openFileDialog->ShowModal() != wxID_CANCEL) textEdit->LoadFile(openFileDialog->GetPath());
  evt.Skip();
}

void EditorGUIMain::onSaveClicked(wxCommandEvent &evt)
{
  save();
  evt.Skip();
}

void EditorGUIMain::onSaveAsClicked(wxCommandEvent &evt)
{
  saveAs();
  evt.Skip();
}

void EditorGUIMain::onExportClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onExportAsClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onExitClicked(wxCommandEvent &evt)
{
  if(!Global::_SAVED)
  {
    int decision = notSaved->ShowModal();
    if(decision == wxID_NO) Close(TRUE);
    else if(decision == wxID_YES) onSaveClicked(evt);
    else evt.Skip();
  }
  else Close(TRUE);
}

void EditorGUIMain::onBGClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onFontClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onColorClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onTitleClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onSlideClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onImageClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onSubtitleClicked(wxCommandEvent &evt) { }
void EditorGUIMain::onSubpointClicked(wxCommandEvent &evt) { }

void EditorGUIMain::onTextChanged(wxCommandEvent &evt)
{
  Global::_SAVED = false;
}

void gprintf(std::string format, ...) {}
void changeStatus(std::string status) {}
void progress() {}

void clearTerminal() {}
void resetProgress() {}