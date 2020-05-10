#include "lib/gui.hh"
#include "lib/presentCreator.hh"

GtkWidget *window;
GtkWidget *about;

GtkWidget *loadBrowse;
GtkWidget *saveBrowse;
GtkFileFilter *loadFilter;
GtkFileFilter *saveFilter;

GtkWidget *title;
GtkWidget *aboutBtn;
GtkWidget *closeBtn;
GtkWidget *createBtn;
GtkWidget *browseBtnInput;
GtkWidget *browseBtnOutput;
GtkWidget *pathEntryInput;
GtkWidget *pathEntryOutput;
GtkWidget *progress;
GtkWidget *consoleOut;
GtkWidget *aboutCloseBtn;

GtkBuilder *builder;
GtkTextBuffer *buffer;
GtkTextIter bufferEnd;

GtkCssProvider *cssProvider;

int gtkInit(int* argc, char** argv[])
{
  gtk_init(argc, argv);
  builder = gtk_builder_new_from_file("dat/GTK.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
  about = GTK_WIDGET(gtk_builder_get_object(builder, "about"));
  loadBrowse = gtk_file_chooser_dialog_new("Select input file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel",GTK_RESPONSE_CANCEL, "_Select", GTK_RESPONSE_ACCEPT, NULL);
  saveBrowse = gtk_file_chooser_dialog_new("Select output file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel",GTK_RESPONSE_CANCEL, "_Select", GTK_RESPONSE_ACCEPT, NULL);

  loadFilter = gtk_file_filter_new();
  saveFilter = gtk_file_filter_new();

  title = GTK_WIDGET(gtk_builder_get_object(builder, "title"));
  closeBtn = GTK_WIDGET(gtk_builder_get_object(builder, "mainCloseBtn"));
  aboutBtn = GTK_WIDGET(gtk_builder_get_object(builder, "aboutBtn"));
  createBtn = GTK_WIDGET(gtk_builder_get_object(builder, "createBtn"));
  browseBtnInput = GTK_WIDGET(gtk_builder_get_object(builder, "browseBtnInput"));
  browseBtnOutput = GTK_WIDGET(gtk_builder_get_object(builder, "browseBtnOutput"));
  pathEntryInput = GTK_WIDGET(gtk_builder_get_object(builder, "pathEntryInput"));
  pathEntryOutput = GTK_WIDGET(gtk_builder_get_object(builder, "pathEntryOutput"));
  aboutCloseBtn = GTK_WIDGET(gtk_builder_get_object(builder, "aboutCloseBtn"));
  progress = GTK_WIDGET(gtk_builder_get_object(builder, "progress"));
  consoleOut = GTK_WIDGET(gtk_builder_get_object(builder, "consoleOut"));

  g_signal_connect(aboutCloseBtn, "clicked", G_CALLBACK(on_aboutCloseBtn_clicked), NULL);
  g_signal_connect(about, "delete-event", G_CALLBACK(on_aboutCloseBtn_clicked), NULL);
  g_signal_connect(closeBtn, "clicked", G_CALLBACK(on_mainCloseBtn_clicked), NULL);
  g_signal_connect(aboutBtn, "clicked", G_CALLBACK(on_aboutBtn_clicked), NULL);
  g_signal_connect(createBtn, "clicked", G_CALLBACK(on_createBtn_clicked), NULL);
  g_signal_connect(browseBtnInput, "clicked", G_CALLBACK(on_browseBtnInput_clicked), NULL);
  g_signal_connect(browseBtnOutput, "clicked", G_CALLBACK(on_browseBtnOutput_clicked), NULL);
  g_signal_connect(window, "delete-event", G_CALLBACK(on_mainCloseBtn_clicked), NULL);

  gtk_file_filter_set_name(loadFilter, "Input text file (.txt)");
  gtk_file_filter_add_mime_type(loadFilter, "text/plain");
  gtk_file_filter_set_name(saveFilter, "PDF Document (.pdf)");
  gtk_file_filter_add_mime_type(saveFilter, "application/pdf");

  gtk_label_set_text(GTK_LABEL(title), Global::_VERSIONSTRING.c_str());
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about), Global::_VERSIONSTRING.c_str());

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(consoleOut));

  cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "dat/style.css", NULL);
  gtk_style_context_add_provider(gtk_widget_get_style_context(consoleOut), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_builder_connect_signals(builder, NULL);

  gtk_widget_show(window);
  gtk_main();

  gtk_widget_show(window);
  g_object_unref(builder);
  return 0;
}

void on_mainCloseBtn_clicked()
{
  g_signal_emit_by_name(window, "hide");
  gtk_widget_destroy(loadBrowse);
  gtk_widget_destroy(saveBrowse);
  gtk_widget_destroy(about);

  gtk_main_quit();
}

void on_aboutBtn_clicked()
{
  gtk_dialog_run(GTK_DIALOG(about));
}

void on_browseBtnInput_clicked()
{
  gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(loadBrowse), loadFilter);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(loadBrowse), FALSE);

  if (gtk_dialog_run(GTK_DIALOG(loadBrowse)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(loadBrowse));
    gtk_entry_set_text(GTK_ENTRY(pathEntryInput), filename);
    g_free(filename);
  }

  gtk_widget_hide(loadBrowse);
}

void on_browseBtnOutput_clicked()
{
  gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(saveBrowse), saveFilter);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(saveBrowse), TRUE);
  gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(saveBrowse), "Untitled.pdf");

  if (gtk_dialog_run(GTK_DIALOG(saveBrowse)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(saveBrowse));
    gtk_entry_set_text(GTK_ENTRY(pathEntryOutput), filename);
    g_free(filename);
  }

  gtk_widget_hide(saveBrowse);
}

void on_aboutCloseBtn_clicked()
{
  g_signal_emit_by_name(about, "hide");
  g_signal_emit_by_name(window, "set-focus");
}

void on_createBtn_clicked()
{
  createPresent(gtk_entry_get_text(GTK_ENTRY(pathEntryInput)), gtk_entry_get_text(GTK_ENTRY(pathEntryOutput)));
  gtk_widget_set_sensitive(createBtn, FALSE);
  gtk_widget_set_sensitive(browseBtnInput, FALSE);
  gtk_widget_set_sensitive(browseBtnOutput, FALSE);
  gtk_widget_set_sensitive(pathEntryInput, FALSE);
  gtk_widget_set_sensitive(pathEntryOutput, FALSE);
  gprintf("You may now close this application\n");
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
    gtk_text_buffer_get_end_iter(buffer, &bufferEnd);
    gtk_text_buffer_insert(buffer, &bufferEnd, tmp.c_str() , -1);

    while (gtk_events_pending ()) gtk_main_iteration ();
  }

  va_end(args);
}