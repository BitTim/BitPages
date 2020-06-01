#include "lib/guiActions.hh"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/wfstream.h>

#include "lib/globals.hh"
#include "lib/imageGenerator.hh"
#include "lib/parser.hh"
#include "lib/gui.hh"

void update(wxStyledTextCtrl* textEdit)
{
	programChanged = true;
	long cursorLine = textEdit->GetInsertionPoint();
	textEdit->PositionToXY(cursorLine, NULL, &cursorLine);

	if (Global::_SAVEFILENAME == "") Global::_SAVEFILENAME = "Presentation.txt";
	if (!Global::_SAVED && Global::_SAVEPATH == Global::_CACHEPATH / "Editor") textEdit->SaveFile(std::string(Global::_SAVEPATH + "/" + Global::_SAVEFILENAME));
	if (Global::_SAVEPATH == "") Global::_SAVEPATH = (Global::_CACHEPATH / "Editor").string();

	std::vector<Token> tokens = tokenize(textEdit->GetValue());
	parse(tokens, cursorLine);

	if (Global::_CSLIDEPREVIEW != Global::_PREVSLIDEPREVIEW || Global::_FORCEUPDATE)
	{
		Global::_PREVSLIDEPREVIEW = Global::_CSLIDEPREVIEW;
		Global::_FORCEUPDATE = false;

		//Syntax Highlighting
		for (int i = 0; i < tokens.size(); i++)
		{
			std::string buffer = std::string(textEdit->GetValue());
			int tokenStart = buffer.find(tokens[i].value);
			int tokenEnd = tokenStart + tokens[i].value.length();

			if (tokens[i].value[0] == '.') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x29, 0x3C, 0x7A), *wxWHITE));
			else if (tokens[i].value[0] == '*') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x21, 0x54, 0x63), *wxWHITE));
			else if (tokens[i].value[0] == '<' && tokens[i].value[tokens[i].value.length() - 1] == '>') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0x43, 0x6F, 0x25), *wxWHITE));
			else if (tokens[i].value[0] == '[' && tokens[i].value[tokens[i].value.length() - 1] == ']') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0xA1, 0x36, 0x42), *wxWHITE));
			else if (tokens[i].value[0] == '-' && tokens[i].value[tokens[i].value.length() - 1] == '-') textEdit->SetStyle(tokenStart, tokenEnd, wxTextAttr(wxColor(0xC3, 0x99, 0x4B), *wxWHITE));
			else textEdit->SetStyle(tokenStart, tokenEnd < textEdit->GetLastPosition() ? tokenEnd + 1 : tokenEnd, wxTextAttr(wxColor(0x10, 0x09, 0x1B), *wxWHITE));
		}

		//Warnings and Errors
		warnings->Clear();

		for (int i = 0; i < Global::_ERRORS.size(); i++)
		{
			long cursorStart;
			long cursorEnd;

			if (Global::_ERRORS[i].line != -1)
			{
				cursorStart = textEdit->XYToPosition(0, Global::_ERRORS[i].line - 1);
				cursorEnd = cursorStart + textEdit->GetLineLength(Global::_ERRORS[i].line - 1);
			}

			if (Global::_ERRORS[i].type == ERROR_ERROR)
			{
				warnings->SetDefaultStyle(wxTextAttr(wxColor(0xC0, 0x3B, 0x3A)));
				if (Global::_ERRORS[i].line != -1) textEdit->SetStyle(cursorStart, cursorEnd, wxTextAttr(wxNullColour, wxColor(0xDE, 0x9C, 0x9C)));
			}

			if (Global::_ERRORS[i].type == ERROR_WARNING)
			{
				warnings->SetDefaultStyle(wxTextAttr(wxColor(0xF5, 0xD5, 0x57)));
				if (Global::_ERRORS[i].line != -1) textEdit->SetStyle(cursorStart, cursorEnd, wxTextAttr(wxNullColour, wxColor(0xDD, 0xC7, 0x98)));
			}

			if (Global::_ERRORS[i].line != -1) warnings->AppendText("Line " + std::to_string(Global::_ERRORS[i].line) + ": ");
			warnings->AppendText(Global::_ERRORS[i].desc + "\n");
		}

		if (Global::_ERRORS.size() == 0)
		{
			warnings->SetDefaultStyle(wxTextAttr(wxColor(0x89, 0xB1, 0x7E)));
			warnings->AppendText("No warnings or errors");
		}
	}

	programChanged = false;
}

void previewUpdate(Presentation& present, int slide)
{
	if (slide < present.slides.size())
	{
		saveImage(generateSurface(slide), (Global::_CACHEPATH / "preview.png").string());
		guiApp->mainFrame->setPreview();
	}
	else guiApp->mainFrame->clearPreview();
}

void save(bool saveAs = false)
{

}

void exportPDF(bool exportAs = false)
{

}