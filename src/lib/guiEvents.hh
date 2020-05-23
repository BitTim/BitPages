#pragma once

bool OnInit();
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