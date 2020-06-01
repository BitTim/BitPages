#include <algorithm>

#include "lib/gui.hh"
#include "lib/globals.hh"

bool programChanged = false;
long prevCursorPos;
GUIApp* guiApp;

GUIMain::GUIMain() : wxFrame(nullptr, wxID_ANY, "")
{
	pngHandler = new wxPNGHandler();

	idle = new wxTimer(this, 10014);
	cursor = new wxTimer(this, 10015);

	initMenuBar();
	initLayout();
	initStyles();
	initDialogs();

	clearPreview();

	wxIcon icon;
	icon.LoadFile("dat/icon.ico", wxBITMAP_TYPE_ICO);
	SetIcons(icon);
}

void runGUI(Presentation &present, int argc, char* argv[])
{
	guiApp = new GUIApp();
	wxApp::SetInstance(guiApp);
	wxEntry(argc, argv);
	wxEntryCleanup();
}

void GUIMain::initMenuBar()
{
	std::vector<wxMenu*> menus;
	menus.push_back(createMenu(
		{ wxID_NEW, wxID_OPEN, wxID_SAVE, wxID_SAVEAS, 10001, 10002, wxID_EXIT },
		{ "&New", "&Open", "&Save", "Save &As...", "&Export", "Expo&rt As...", "E&xit" },
		{ 3, 5 }
	));

	menus.push_back(createMenu(
		{ 10003, 10004, 10005, 10006, 1007, 10008, 10009, 10010 },
		{ "&Background Image", "&Font", "Text &Color", "&Title Slide", "&Slide", "&Image", "Su&btitle", "Sub&point" },
		{ 2, 4 }
	));

	menus.push_back(createMenu(
		{ 10016, 10017 },
		{ "&Documentation", "&About" },
		{ 0 }
	));

	this->SetMenuBar(createMenuBar(
		menus,
		{ "&File", "&Insert", "&Help" }
	));
}

wxMenu* GUIMain::createMenu(std::vector<int> ids, std::vector<std::string> entries, std::vector<int> seperators)
{
	wxMenu* menu = new wxMenu();

	if (ids.size() != entries.size()) return nullptr;

	for (int i = 0; i < entries.size(); i++)
	{
		menu->Append(ids[i], _(entries[i]));
		if (std::find(seperators.begin(), seperators.end(), i) != seperators.end()) menu->AppendSeparator();
	}

	return menu;
}

wxMenuBar* GUIMain::createMenuBar(std::vector<wxMenu*> menus, std::vector<std::string> entries)
{
	wxMenuBar* menuBar;

	if (menus.size == entries.size())
	{
		for (int i = 0; i < menus.size(); i++) menuBar->Append(menus[i], _(entries[i]));
	}

	return menuBar;
}

void GUIMain::initLayout()
{
	textEdit = new wxStyledTextCtrl(this, 10011);
	preview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(), wxDefaultPosition, wxSize(640, 360));
	warnings = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, (wxTE_MULTILINE | wxTE_RICH2 | wxTE_READONLY | wxHSCROLL));

	SetSizerAndFit(createMainLayout());
}

wxBoxSizer* GUIMain::createMainLayout()
{
	wxBoxSizer* boxSizer = new wxBoxSizer(wxHORIZONTAL);

	boxSizer->Add(textEdit, 1, wxEXPAND | wxALL, 10);
	boxSizer->Add(createRightLayout(), 0, wxEXPAND | wxALL, 10);

	return boxSizer;
}

wxBoxSizer* GUIMain::createRightLayout()
{
	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	wxButton* updateBtn = new wxButton(this, 10018, "Update Preview");

	boxSizer->Add(preview, 0, wxBOTTOM, 10);
	boxSizer->Add(updateBtn, 0, wxBOTTOM | wxALIGN_CENTER, 10);
	boxSizer->Add(warnings, 1, wxEXPAND | wxTOP, 10);

	return boxSizer;
}

void GUIMain::initStyles()
{
	wxFont font = textEdit->GetFont();
	font.SetFamily(wxFONTFAMILY_TELETYPE);
	textEdit->SetFont(font);
	textEdit->SetBackgroundColour(*wxWHITE);
	textEdit->SetMarginType(10, wxSTC_MARGIN_NUMBER);

	font = warnings->GetFont();
	font.SetFamily(wxFONTFAMILY_TELETYPE);
	warnings->SetFont(font);
	warnings->SetBackgroundColour(wxColor(0x33, 0x33, 0x33));
}

void GUIMain::initDialogs()
{
	openFileDialog = new wxFileDialog(this, _("Open presentation"), wxEmptyString, wxEmptyString, "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	saveFileDialog = new wxDirDialog(this, _("Select folder to save presentation to"), wxEmptyString);
	exportFileDialog = new wxFileDialog(this, _("Select an input file"), wxEmptyString, "Presentation.pdf", "PDF documents (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	openImageDialog = new wxFileDialog(this, _("Open image"), wxEmptyString, wxEmptyString, "Image files (*.png;*.jpg;*.webp)|*.png;*.jpg;*.webp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	openFontDialog = new wxFileDialog(this, _("Open font"), wxEmptyString, wxEmptyString, "Font files (*.ttf)|*.ttf", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	selectColorDialog = new wxColourDialog(this);

	notSaved = new wxMessageDialog(this, "The file contains unsaved changes. Save changes?", "Unsaved file", wxYES_NO | wxCANCEL | wxNO_DEFAULT | wxICON_WARNING);
}

void GUIMain::clearPreview()
{
	wxFileInputStream previewIn(std::string("dat/emptyPreview.png"));
	wxImage data;

	pngHandler->LoadFile(&data, previewIn);
	data.Rescale(preview->GetSize().x, preview->GetSize().y);
	preview->SetBitmap(data);
}

void GUIMain::setPreview()
{
	wxFileInputStream previewIn((Global::_CACHEPATH / "preview.png").string());
	wxImage data;

	pngHandler->LoadFile(&data, previewIn);

	if (data.IsOk())
	{
		data.Rescale(preview->GetMinSize().x, preview->GetMinSize().y);
		preview->SetBitmap(data);
	}
	else clearPreview();
}

wxPNGHandler* GUIMain::getPNGHandler() { return pngHandler; }
wxTimer* GUIMain::getCursorTimer() { return cursor; }
long GUIMain::getCursorPos() { return textEdit->GetInsertionPoint(); }

//=============================
// Events
//=============================
void EditorGUIMain::update()
{
	
}

void EditorGUIMain::timedUpdate(wxTimerEvent &evt)
{
  Global::_FORCEUPDATE = true;
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

void EditorGUIMain::onUpdateClicked(wxCommandEvent& evt)
{
	previewUpdate();
	evt.Skip();
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
  if(Global::_SAVEPATH == Global::_CACHEPATH / "Editor") saveAs();
  else
  {
	if (Global::_SAVEFILENAME == "") Global::_SAVEFILENAME = "Presentation.txt";
	textEdit->SaveFile(Global::_SAVEPATH + "/" + Global::_SAVEFILENAME);
	Global::_SAVED = true;
  }
}

void EditorGUIMain::saveAs()
{
  if(saveFileDialog->ShowModal() != wxID_CANCEL)
  {
	std::string prevPath = Global::_SAVEPATH;
	Global::_SAVEPATH = saveFileDialog->GetPath();

	if(prevPath == Global::_CACHEPATH / "Editor")
	{
	  fs::copy(Global::_CACHEPATH / "Editor" / "Images", Global::_SAVEPATH + "/Images");
	  fs::copy(Global::_CACHEPATH / "Editor" / "Fonts", Global::_SAVEPATH + "/Fonts");
	}

	if (Global::_SAVEFILENAME == "") Global::_SAVEFILENAME = "Presentation.txt";
	textEdit->SaveFile(Global::_SAVEPATH + "/" + Global::_SAVEFILENAME);
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
	  Global::_SAVEPATH = openFileDialog->GetDirectory();

	  std::string inpath = std::string(openFileDialog->GetPath());
	  int fnsep = inpath.find_last_of("/\\");
	  std::string filename = inpath.substr(fnsep + 1, std::string::npos);
	  
	  if (filename == "") Global::_SAVEFILENAME = "Presentation.txt";
	  textEdit->LoadFile(Global::_SAVEPATH + "/" + filename);
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

void EditorGUIMain::onTextChanged(wxCommandEvent& evt)
{
	Global::_SAVED = false;
	if (!programChanged) idle->Start(300, true);

	evt.Skip();
}