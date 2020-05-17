#include <algorithm>

#include "lib/gui.hh"
#include "lib/globals.hh"
#include "lib/parser.hh"
#include "lib/imageGenerator.hh"
#include "lib/presentCreator.hh"

wxBEGIN_EVENT_TABLE(EditorGUIMain, wxFrame)
  EVT_CLOSE(EditorGUIMain::OnClose)
  EVT_SIZING(EditorGUIMain::resizing)
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

  pngHandler = new wxPNGHandler();

  boxSizer = new wxBoxSizer(wxHORIZONTAL);
  textEdit = new wxTextCtrl(this , 10011, wxEmptyString, wxDefaultPosition, wxDefaultSize, (wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL));
  previewImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap());
  previewImage->SetMinSize(wxSize(640, 360));
  previewImageData = wxImage();

  warnings = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, (wxTE_MULTILINE | wxTE_RICH2 | wxTE_READONLY | wxHSCROLL));

  idle = new wxTimer(this, 10014);
  cursor = new wxTimer(this, 10015);

  preview = new wxBoxSizer(wxVERTICAL);

  preview->Add(previewImage, 0, wxBOTTOM, 10);
  preview->Add(warnings, 1, wxEXPAND | wxTOP | wxALIGN_CENTER, 10);

  boxSizer->Add(textEdit, 1, wxEXPAND | wxALL, 10);
  boxSizer->Add(preview, 0, wxEXPAND | wxALL, 10);
  SetSizerAndFit(boxSizer);

  wxFont font = textEdit->GetFont();
  font.SetFamily(wxFONTFAMILY_TELETYPE);
  textEdit->SetFont(font);

  font = warnings->GetFont();
  font.SetFamily(wxFONTFAMILY_TELETYPE);
  warnings->SetFont(font);
  warnings->SetBackgroundColour(wxColor(0x33, 0x33, 0x33));

  openFileDialog = new wxFileDialog(this, _("Open presentation"), wxEmptyString, wxEmptyString, "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  saveFileDialog = new wxFileDialog(this, _("Save presentation"), wxEmptyString, "Presentation.txt", "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  exportFileDialog = new wxFileDialog(this, _("Select an input file"), wxEmptyString, "Presentation.pdf", "PDF Documents (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  notSaved = new wxMessageDialog(this, "The file contains unsaved changes. Save changes?", "Unsaved file", wxYES_NO | wxCANCEL | wxNO_DEFAULT | wxICON_WARNING);
  error = new wxMessageDialog(this, wxEmptyString, wxEmptyString, wxICON_ERROR);

  wxIcon icon;
  icon.LoadFile("dat/BPLogo_Editor.png", wxBITMAP_TYPE_PNG);
  SetIcons(icon);

  wxFileInputStream previewIn(std::string("dat/emptyPreview.png"));
  pngHandler->LoadFile(&previewImageData, previewIn);

  previewImageData.Rescale(previewImage->GetMinSize().x, previewImage->GetMinSize().y);    
  previewImage->SetBitmap(previewImageData);
}


//=============================
// Events
//=============================

void EditorGUIMain::loadPreview()
{
  wxFileInputStream previewIn(std::string(Global::_CACHEPATH / "preview.png"));
  pngHandler->LoadFile(&previewImageData, previewIn);

  previewImageData.Rescale(previewImage->GetMinSize().x, previewImage->GetMinSize().y);    
  previewImage->SetBitmap(previewImageData);
}

void EditorGUIMain::update()
{
  std::string input = std::string(textEdit->GetValue());
  int cursorLine = std::count(input.begin(), input.begin() + textEdit->GetInsertionPoint(), '\n') + 1;

  std::vector<Token> tokens = tokenize(input);
  parse(tokens, cursorLine);

  if(Global::_CSLIDEPREVIEW != Global::_PREVSLIDEPREVIEW || Global::_FORCEUPDATE)
  {
    Global::_PREVSLIDEPREVIEW = Global::_CSLIDEPREVIEW;
    Global::_FORCEUPDATE = false;

    previewUpdate();

    warnings->Clear();

    for(int i = 0; i < Global::_ERRORS.size(); i++)
    {
      if(Global::_ERRORS[i].type == ERROR_ERROR) warnings->SetDefaultStyle(wxTextAttr(wxColor(0xC0, 0x3B, 0x3A)));
      if(Global::_ERRORS[i].type == ERROR_WARNING) warnings->SetDefaultStyle(wxTextAttr(wxColor(0xF5, 0xD5, 0x57)));

      warnings->AppendText("Line " + std::to_string(Global::_ERRORS[i].line) + ": " + Global::_ERRORS[i].desc + "\n");
    }

    if(Global::_ERRORS.size() == 0)
    {
      warnings->SetDefaultStyle(wxTextAttr(wxColor(0x89, 0xB1, 0x7E)));
      warnings->AppendText("No warnings or errors");
    }

    for(int i = 0; i < tokens.size(); i++)
    {
      std::string buffer = std::string(textEdit->GetValue());
      int tokenStart = buffer.find(tokens[i].value);
      int tokenEnd = tokenStart + tokens[i].value.length() + 1;

      if(tokens[i].value[0] == '.') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x29, 0x3C, 0x7A)));
      else if(tokens[i].value[0] == '*') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x21, 0x54, 0x63)));
      else if(tokens[i].value[0] == '<' && tokens[i].value[tokens[i].value.length() - 1] == '>') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x43, 0x6F, 0x25)));
      else if(tokens[i].value[0] == '[' && tokens[i].value[tokens[i].value.length() - 1] == ']') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0xA1, 0x36, 0x42)));
      else if(tokens[i].value[0] == '-' && tokens[i].value[tokens[i].value.length() - 1] == '-') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0xC3, 0x99, 0x4B)));
      else textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x10, 0x09, 0x1B)));
    }
  }
}

void EditorGUIMain::previewUpdate()
{
  if(Global::_CSLIDEPREVIEW < Global::_PRESENT->slides.size())
  {
    saveImage(generateSurface(Global::_CSLIDEPREVIEW), Global::_CACHEPATH / "preview.png");
    loadPreview();
  }
}

void EditorGUIMain::timedUpdate(wxTimerEvent &evt)
{
  update();
  evt.Skip();
}

void EditorGUIMain::cursorUpdate(wxMouseEvent &evt)
{
  update();
  evt.Skip();
}

void EditorGUIMain::timedCursorUpdate(wxTimerEvent &evt)
{
  std::string input = std::string(textEdit->GetValue());
  int cursorLine = std::count(input.begin(), input.begin() + textEdit->GetInsertionPoint(), '\n') + 1;
  if(cursorLine != prevCursorPos)
  {
    prevCursorPos = cursorLine;
    update();
  }
  evt.Skip();
}

bool EditorGUIApp::OnInit()
{
  mainFrame = new EditorGUIMain();
  mainFrame->SetMinSize(wxSize(800, 600));
  mainFrame->SetSize(mainFrame->GetMinSize());
  mainFrame->SetTitle(Global::_VERSIONSTRING);
  mainFrame->Show();
  mainFrame->Maximize();

  //Load default font
	Global::_DEFAULTFONT = { {"title", TTF_OpenFont("dat/defaultFont.ttf", 68)}, {"subtitle", TTF_OpenFont("dat/defaultFont.ttf", 46)}, {"normal", TTF_OpenFont("dat/defaultFont.ttf", 34)}, {"footer", TTF_OpenFont("dat/defaultFont.ttf", 12)} };
	if (Global::_DEFAULTFONT["title"] == NULL || Global::_DEFAULTFONT["subtitle"] == NULL || Global::_DEFAULTFONT["normal"] == NULL)
	{
		printf("[ERROR]: Loading font: %s\n", TTF_GetError());
    mainFrame->error->SetTitle("Error loading default font");
    mainFrame->error->SetMessage("Could not load default Font. Make sure the \"dat\" folder is present in the installation folder of BitPresent");
    mainFrame->error->ShowModal();
		return false;
	}
	Global::_PRESENT->font = Global::_DEFAULTFONT;

  mainFrame->prevCursorPos = mainFrame->textEdit->GetInsertionPoint();
  mainFrame->cursor->Start(1);

  return true;
}

void EditorGUIMain::OnClose(wxCloseEvent& evt)
{
  if (evt.CanVeto())
  {
    if(checkSaved()) Destroy();
    else evt.Veto();
  }
  else Destroy();
}

void EditorGUIMain::resizing(wxSizeEvent &evt)
{
  previewUpdate();
  evt.Skip();
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

void EditorGUIMain::exportPDF()
{
  if(Global::_SAVEPATH == "") exportPDFAs();
  else
  {
    //createPresent();
  }
}

void EditorGUIMain::exportPDFAs()
{
  if(saveFileDialog->ShowModal() != wxID_CANCEL)
  {
    Global::_EXPORTPATH = saveFileDialog->GetPath();

  }
}

bool EditorGUIMain::checkSaved()
{
  if(!Global::_SAVED)
  {
    int decision = notSaved->ShowModal();
    if(decision == wxID_NO)
    {
      Global::_SAVED = true;
      return true;
    }
    else if(decision == wxID_YES)
    {
      save();
      return true;
    }
    else return false;
  }
  return true;
}

void EditorGUIMain::onNewClicked(wxCommandEvent &evt)
{
  if(checkSaved())
  {
    textEdit->Clear();
    Global::_SAVEPATH = "";
    Global::_EXPORTPATH = "";
  }
  evt.Skip();
}

void EditorGUIMain::onOpenClicked(wxCommandEvent &evt)
{
  if(checkSaved())
  {
    if(openFileDialog->ShowModal() != wxID_CANCEL)
    {
      Global::_SAVEPATH = openFileDialog->GetPath();
      textEdit->LoadFile(Global::_SAVEPATH);
      Global::_SAVED = true;
    }
  }
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
  if(checkSaved())
  {
    Close(TRUE);
  }
  evt.Skip();
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
  Global::_FORCEUPDATE = true;
  idle->Start(300, true);
  evt.Skip();
}

//Compatibility with common libs
void gprintf(std::string format, ...) {}
void changeStatus(std::string status) {}
void progress() {}

void clearTerminal() {}
void resetProgress() {}