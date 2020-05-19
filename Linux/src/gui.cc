#include <algorithm>
#include <thread>

#include "lib/gui.hh"
#include "lib/globals.hh"
#include "lib/parser.hh"
#include "lib/imageGenerator.hh"
#include "lib/pdfCreator.hh"

wxBEGIN_EVENT_TABLE(EditorGUIMain, wxFrame)
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

  EVT_MENU(10016, EditorGUIMain::onDocsClicked)
  EVT_MENU(10017, EditorGUIMain::onAboutClicked)

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

  helpMenu = new wxMenu();
  helpMenu->Append(10016, _("&Documentation"));
  helpMenu->AppendSeparator();
  helpMenu->Append(10017, _("&About"));

  menuBar = new wxMenuBar();
  menuBar->Append(fileMenu, _("&File"));
  menuBar->Append(insertMenu, _("&Insert"));
  menuBar->Append(helpMenu, _("&Help"));
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
  preview->Add(warnings, 1, wxEXPAND | wxTOP, 10);

  boxSizer->Add(textEdit, 1, wxEXPAND | wxALL, 10);
  boxSizer->Add(preview, 0, wxEXPAND | wxALL, 10);
  SetSizerAndFit(boxSizer);

  wxFont font = textEdit->GetFont();
  font.SetFamily(wxFONTFAMILY_TELETYPE);
  textEdit->SetFont(font);
  textEdit->SetBackgroundColour(*wxWHITE);

  font = warnings->GetFont();
  font.SetFamily(wxFONTFAMILY_TELETYPE);
  warnings->SetFont(font);
  warnings->SetBackgroundColour(wxColor(0x33, 0x33, 0x33));

  openFileDialog = new wxFileDialog(this, _("Open presentation"), wxEmptyString, wxEmptyString, "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  saveFileDialog = new wxDirDialog(this, _("Select folder to save presentation to"), wxEmptyString);
  exportFileDialog = new wxFileDialog(this, _("Select an input file"), wxEmptyString, "Presentation.pdf", "PDF documents (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  openImageDialog = new wxFileDialog(this, _("Open image"), wxEmptyString, wxEmptyString, "Image files (*.png;*.jpg;*.webp)|*.png;*.jpg;*.webp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  openFontDialog = new wxFileDialog(this, _("Open font"), wxEmptyString, wxEmptyString, "Font files (*.ttf)|*.ttf", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  selectColorDialog = new wxColourDialog(this);

  notSaved = new wxMessageDialog(this, "The file contains unsaved changes. Save changes?", "Unsaved file", wxYES_NO | wxCANCEL | wxNO_DEFAULT | wxICON_WARNING);
  error = new wxMessageDialog(this, wxEmptyString, wxEmptyString, wxICON_ERROR);

  wxIcon icon;
  icon.LoadFile("dat/Logo.png", wxBITMAP_TYPE_PNG);
  SetIcons(icon);

  wxFileInputStream previewIn(std::string("dat/emptyPreview.png"));
  pngHandler->LoadFile(&previewImageData, previewIn);

  previewImageData.Rescale(previewImage->GetMinSize().x, previewImage->GetMinSize().y);    
  previewImage->SetBitmap(previewImageData);
}


//=============================
// Events
//=============================

void EditorGUIMain::update()
{
  std::string input = std::string(textEdit->GetValue());
  int cursorLine = std::count(input.begin(), input.begin() + textEdit->GetInsertionPoint(), '\n') + 1;

  if(!Global::_SAVED && Global::_SAVEPATH == Global::_CACHEPATH / "Editor" / "current.txt") textEdit->SaveFile(std::string(Global::_SAVEPATH));
  if(Global::_SAVEPATH == "") Global::_SAVEPATH = (Global::_CACHEPATH / "Editor" / "current.txt").string();

  std::vector<Token> tokens = tokenize(input);
  parse(tokens, cursorLine);

  if(Global::_CSLIDEPREVIEW != Global::_PREVSLIDEPREVIEW || Global::_FORCEUPDATE)
  {
    Global::_PREVSLIDEPREVIEW = Global::_CSLIDEPREVIEW;
    Global::_FORCEUPDATE = false;

    Global::_PREVIEWTHREAD = std::thread(&EditorGUIMain::previewUpdate, this);
    Global::_PREVIEWTHREAD.detach();

    //Syntax Highlighting
    for(int i = 0; i < tokens.size(); i++)
    {
      std::string buffer = std::string(textEdit->GetValue());
      int tokenStart = buffer.find(tokens[i].value);
      int tokenEnd = tokenStart + tokens[i].value.length();

      if(tokens[i].value[0] == '.') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x29, 0x3C, 0x7A), *wxWHITE));
      else if(tokens[i].value[0] == '*') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x21, 0x54, 0x63), *wxWHITE));
      else if(tokens[i].value[0] == '<' && tokens[i].value[tokens[i].value.length() - 1] == '>') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x43, 0x6F, 0x25), *wxWHITE));
      else if(tokens[i].value[0] == '[' && tokens[i].value[tokens[i].value.length() - 1] == ']') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0xA1, 0x36, 0x42), *wxWHITE));
      else if(tokens[i].value[0] == '-' && tokens[i].value[tokens[i].value.length() - 1] == '-') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0xC3, 0x99, 0x4B), *wxWHITE));
      else textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x10, 0x09, 0x1B), *wxWHITE));
    }

    //Warnings and Errors
    warnings->Clear();

    for(int i = 0; i < Global::_ERRORS.size(); i++)
    {
      long cursorStart;
      long cursorEnd;

      if(Global::_ERRORS[i].line != -1)
      {
       cursorStart = textEdit->XYToPosition(0, Global::_ERRORS[i].line - 1);
       cursorEnd = cursorStart + textEdit->GetLineLength(Global::_ERRORS[i].line - 1);
      }

      if(Global::_ERRORS[i].type == ERROR_ERROR)
      {
        warnings->SetDefaultStyle(wxTextAttr(wxColor(0xC0, 0x3B, 0x3A)));
        if(Global::_ERRORS[i].line != -1) textEdit->SetStyle(cursorStart, cursorEnd, wxTextAttr(wxNullColour, wxColor(0xDE, 0x9C, 0x9C)));
      }

      if(Global::_ERRORS[i].type == ERROR_WARNING)
      {
        warnings->SetDefaultStyle(wxTextAttr(wxColor(0xF5, 0xD5, 0x57)));
        if(Global::_ERRORS[i].line != -1) textEdit->SetStyle(cursorStart, cursorEnd, wxTextAttr(wxNullColour, wxColor(0xDD, 0xC7, 0x98)));
      }

      if(Global::_ERRORS[i].line != -1) warnings->AppendText("Line " + std::to_string(Global::_ERRORS[i].line) + ": ");
      warnings->AppendText(Global::_ERRORS[i].desc + "\n");
    }

    if(Global::_ERRORS.size() == 0)
    {
      warnings->SetDefaultStyle(wxTextAttr(wxColor(0x89, 0xB1, 0x7E)));
      warnings->AppendText("No warnings or errors");
    }
  }
}

void EditorGUIMain::previewUpdate()
{
  if(Global::_CSLIDEPREVIEW < Global::_PRESENT->slides.size() && !Global::_LOCKPREVIEWIMAGE)
  {
    Global::_LOCKPREVIEWIMAGE = true;
    saveImage(generateSurface(Global::_CSLIDEPREVIEW), (Global::_CACHEPATH / "preview.png").string());

    wxFileInputStream previewIn((Global::_CACHEPATH / "preview.png").string());
    pngHandler->LoadFile(&previewImageData, previewIn);

    if(previewImageData.IsOk())
    {
      previewImageData.Rescale(previewImage->GetMinSize().x, previewImage->GetMinSize().y);    
      previewImage->SetBitmap(previewImageData);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    Global::_LOCKPREVIEWIMAGE = false;
  }
  else
  {
    wxFileInputStream previewIn(std::string("dat/emptyPreview.png"));
    pngHandler->LoadFile(&previewImageData, previewIn);

    previewImageData.Rescale(previewImage->GetMinSize().x, previewImage->GetMinSize().y);    
    previewImage->SetBitmap(previewImageData);
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
	if (Global::_DEFAULTFONT["title"] == NULL || Global::_DEFAULTFONT["subtitle"] == NULL || Global::_DEFAULTFONT["normal"] == NULL || Global::_DEFAULTFONT["footer"] == NULL)
	{
		printf("[ERROR]: Loading font: %s\n", TTF_GetError());
        mainFrame->error->SetTitle("Error loading default font");
        mainFrame->error->SetMessage("Could not load default Font. Make sure the \"dat\" folder is present in the installation folder of BitPresent");
        mainFrame->error->ShowModal();
		return false;
	}
	Global::_PRESENT->font = Global::_DEFAULTFONT;

  mainFrame->prevCursorPos = mainFrame->textEdit->GetInsertionPoint();
  mainFrame->SetBackgroundColour(*wxWHITE);
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

void EditorGUIMain::save()
{
  if(Global::_SAVEPATH == Global::_CACHEPATH / "Editor" / "current.txt") saveAs();
  else
  {
    textEdit->SaveFile(Global::_SAVEPATH + "/Presentation.txt");
    Global::_SAVED = true;
  }
}

void EditorGUIMain::saveAs()
{
  if(saveFileDialog->ShowModal() != wxID_CANCEL)
  {
    std::string prevPath = Global::_SAVEPATH;
    Global::_SAVEPATH = saveFileDialog->GetPath();

    if(prevPath == Global::_CACHEPATH / "Editor" / "current.txt")
    {
      fs::copy(Global::_CACHEPATH / "Editor" / "Images", Global::_SAVEPATH + "/Images");
      fs::copy(Global::_CACHEPATH / "Editor" / "Fonts", Global::_SAVEPATH + "/Fonts");
    }

    textEdit->SaveFile(Global::_SAVEPATH + "/Presentation.txt");
    Global::_SAVED = true;
  }
}

void EditorGUIMain::exportPDF()
{
  if(Global::_EXPORTPATH == "") exportPDFAs();
  else
  {
    wxProgressDialog progress("Exporting Presentation", "Exporting the presentation, please wait", 8, this, wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_REMAINING_TIME);
    progress.Update(0);

    textEdit->Enable(false);
    progress.Update(1);

    std::vector<Token> tokens = tokenize(std::string(textEdit->GetValue()));
    progress.Update(2);
    parse(tokens);
    progress.Update(3);

    for (int i = 0; i < Global::_PRESENT->slides.size(); i++)
	  {
		  SDL_Surface* surface = generateSurface(i);
      progress.Update(4);
		  saveImage(surface, Global::_CACHEPATH.string() + std::to_string(i) + ".png");
      progress.Update(5);
		  SDL_FreeSurface(surface);
      progress.Update(6);
	  }

    createPDF(Global::_EXPORTPATH);
    progress.Update(7);
    textEdit->Enable(true);
    progress.Update(8);
  }
}

void EditorGUIMain::exportPDFAs()
{
  if(exportFileDialog->ShowModal() != wxID_CANCEL)
  {
    Global::_EXPORTPATH = exportFileDialog->GetPath();

    wxProgressDialog progress("Exporting Presentation", "Exporting the presentation, please wait", 8, this, wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_REMAINING_TIME);
    progress.Update(0);

    textEdit->Enable(false);
    progress.Update(1);

    std::vector<Token> tokens = tokenize(std::string(textEdit->GetValue()));
    progress.Update(2);
    parse(tokens);
    progress.Update(3);

    for (int i = 0; i < Global::_PRESENT->slides.size(); i++)
	  {
		  SDL_Surface* surface = generateSurface(i);
      progress.Update(4);
		  saveImage(surface, Global::_CACHEPATH.string() + std::to_string(i) + ".png");
      progress.Update(5);
		  SDL_FreeSurface(surface);
      progress.Update(6);
	  }

    createPDF(Global::_EXPORTPATH);
    progress.Update(7);
    textEdit->Enable(true);
    progress.Update(8);
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


//============================
// File Menu
//============================

void EditorGUIMain::onNewClicked(wxCommandEvent &evt)
{
  if(checkSaved())
  {
    textEdit->Clear();
    Global::_SAVEPATH = (Global::_CACHEPATH / "Editor" / "current.txt").string();
    Global::_EXPORTPATH = "";

    Global::_FORCEUPDATE = true;
    update();

    Global::_SAVED = true;
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

void EditorGUIMain::onExportClicked(wxCommandEvent &evt)
{
  exportPDF();
  evt.Skip();
}

void EditorGUIMain::onExportAsClicked(wxCommandEvent &evt)
{
  exportPDFAs();
  evt.Skip();
}

void EditorGUIMain::onExitClicked(wxCommandEvent &evt)
{
  if(checkSaved())
  {
    Close(TRUE);
  }
  evt.Skip();
}


//============================
// Insert Menu
//============================

void EditorGUIMain::onBGClicked(wxCommandEvent &evt)
{
  if(openImageDialog->ShowModal() != wxID_CANCEL)
  {
    std::string inpath = std::string(openImageDialog->GetPath());
    int fnsep = inpath.find_last_of("/\\");
    std::string filename = inpath.substr(fnsep + 1, std::string::npos);

    std::string savepath = Global::_SAVEPATH;
    fnsep = savepath.find_last_of("/\\");
    savepath = savepath.substr(0, fnsep + 1);

    if(!fs::exists(savepath + "/Images")) fs::create_directory(savepath + "/Images");
    std::string outpath = savepath + "/Images/" + filename;
    fs::copy(inpath, outpath, fs::copy_options::skip_existing);
    
    textEdit->WriteText(".background ");
    textEdit->WriteText(std::string("Images/" + filename));
  }
}

void EditorGUIMain::onFontClicked(wxCommandEvent &evt)
{
  if(openFontDialog->ShowModal() != wxID_CANCEL)
  {
    std::string inpath = std::string(openFontDialog->GetPath());
    int fnsep = inpath.find_last_of("/\\");
    std::string filename = inpath.substr(fnsep + 1, std::string::npos);

    std::string savepath = Global::_SAVEPATH;
    fnsep = savepath.find_last_of("/\\");
    savepath = savepath.substr(0, fnsep + 1);

    if(!fs::exists(savepath + "/Fonts")) fs::create_directory(savepath + "/Fonts");
    std::string outpath = savepath + "/Fonts/" + filename;
    fs::copy(inpath, outpath, fs::copy_options::skip_existing);
    
    textEdit->WriteText(".font ");
    textEdit->WriteText(std::string("Fonts/" + filename));
  }
}

void EditorGUIMain::onColorClicked(wxCommandEvent &evt)
{
  if(selectColorDialog->ShowModal() != wxID_CANCEL)
  {
    wxColor color = selectColorDialog->GetColourData().GetColour();
    char buf[8];

    sprintf(buf, "%02x%02x%02x\n", color.Red(), color.Green(), color.Blue());

    textEdit->WriteText(".color ");
    textEdit->WriteText(std::string(buf));
  }
}

void EditorGUIMain::onTitleClicked(wxCommandEvent &evt) 
{
  long cursor = textEdit->GetInsertionPoint();
  textEdit->WriteText("<Title>");
  textEdit->SetSelection(cursor + 1, cursor + 6);
}

void EditorGUIMain::onSlideClicked(wxCommandEvent &evt) 
{
  long cursor = textEdit->GetInsertionPoint();
  textEdit->WriteText("[Title]");
  textEdit->SetSelection(cursor + 1, cursor + 6);
}

void EditorGUIMain::onImageClicked(wxCommandEvent &evt)
{
  if(openImageDialog->ShowModal() != wxID_CANCEL)
  {
    std::string inpath = std::string(openImageDialog->GetPath());
    int fnsep = inpath.find_last_of("/\\");
    std::string filename = inpath.substr(fnsep + 1, std::string::npos);

    std::string savepath = Global::_SAVEPATH;
    fnsep = savepath.find_last_of("/\\");
    savepath = savepath.substr(0, fnsep + 1);

    if(!fs::exists(savepath + "/Images")) fs::create_directory(savepath + "/Images");
    std::string outpath = savepath + "/Images/" + filename;
    fs::copy(inpath, outpath, fs::copy_options::skip_existing);
    
    textEdit->WriteText(".image ");
    textEdit->WriteText(std::string("Images/" + filename));
  }
}

void EditorGUIMain::onSubtitleClicked(wxCommandEvent &evt) 
{
  long cursor = textEdit->GetInsertionPoint();
  textEdit->WriteText("-Subitle-");
  textEdit->SetSelection(cursor + 1, cursor + 8);
}

void EditorGUIMain::onSubpointClicked(wxCommandEvent &evt) 
{
  long cursor = textEdit->GetInsertionPoint();  
  textEdit->WriteText("* Subpoint");
  textEdit->SetSelection(cursor + 2, cursor + 11);
}


//============================
// About Menu
//============================

void EditorGUIMain::onDocsClicked(wxCommandEvent &evt)
{
  wxLaunchDefaultBrowser("https://github.com/BitTim/BitPresent/blob/master/docs/BitPresent%20-%20Documentation.pdf");
}

void EditorGUIMain::onAboutClicked(wxCommandEvent &evt)
{
  wxImage iconRaw;
  wxFileInputStream iconIn(std::string("dat/Logo.png"));
  pngHandler->LoadFile(&iconRaw, iconIn);
  iconRaw.Rescale(100, 100);

  wxBitmap iconBitmap(iconRaw);
  wxIcon icon;
  icon.CopyFromBitmap(iconBitmap);
  
  wxAboutDialogInfo aboutInfo;
  aboutInfo.SetIcon(icon);
  aboutInfo.SetName("BitPresent");
  aboutInfo.SetVersion(Global::_VERSIONSTRING);
  aboutInfo.SetDescription(_("An application for creating Presentations in PDF format, based on Markdown styled input"));
  aboutInfo.SetCopyright("(C) 2020 BitTim");
  aboutInfo.SetWebSite("https://www.github.com/BitTim/BitPresent/");
  aboutInfo.SetLicence(Global::_LICENSE);
  aboutInfo.AddDeveloper("BitTim");
  aboutInfo.AddDocWriter("BitTim");
  wxAboutBox(aboutInfo);
}

//============================
// Text Changed
//============================

void EditorGUIMain::onTextChanged(wxCommandEvent &evt)
{
  Global::_SAVED = false;
  Global::_FORCEUPDATE = true;
  idle->Start(300, true);
  evt.Skip();
}