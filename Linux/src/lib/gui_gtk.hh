/*#pragma once

#include <gtk/gtk.h>
#include "globals.hh"

extern GtkWidget *window;
extern GtkWidget *loadBrowse;
extern GtkWidget *saveBrowse;
extern GtkWidget *about;

extern GtkWidget *title;
extern GtkWidget *aboutBtn;
extern GtkWidget *closeBtn;
extern GtkWidget *createBtn;
extern GtkWidget *browseBtnInput;
extern GtkWidget *browseBtnOutput;
extern GtkWidget *pathEntryInput;
extern GtkWidget *pathEntryOutput;
extern GtkWidget *progress;
extern GtkWidget *consoleOut;
extern GtkWidget *aboutCloseBtn;

extern GtkBuilder *builder;
extern GtkTextBuffer *buffer;

extern int gtkInit(int* argc, char** argv[]);

extern void on_mainCloseBtn_clicked();
extern void on_aboutBtn_clicked();
extern void on_createBtn_clicked();
extern void on_browseBtnInput_clicked();
extern void on_browseBtnOutput_clicked();

extern void on_browseCancelBtn_clicked();
extern void on_browseOpenBtn_clicked();

extern void on_aboutCloseBtn_clicked();
extern void gprintf(std::string format, ...);*/