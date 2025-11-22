#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <glib.h>
// Callback function for window destroy event - exits the GTK main loop
static void destroy_window_cb(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

// Update status bar with download progress
static void update_download_progress(WebKitDownload* download, GParamSpec* pspec, gpointer data) {
    GtkWidget* status_bar = GTK_WIDGET(data);
    gdouble progress = webkit_download_get_estimated_progress(download);
    gchar* message = g_strdup_printf("Downloading: %.0f%%", progress * 100.0);
    gtk_statusbar_push(GTK_STATUSBAR(status_bar), 0, message);
    g_free(message);
}

// Callback when download finishes
static void download_finished_cb(WebKitDownload* download, gpointer data) {
    GtkWidget* status_bar = GTK_WIDGET(data);
    gtk_statusbar_pop(GTK_STATUSBAR(status_bar), 0);
}

// Callback when download fails
static void download_failed_cb(WebKitDownload* download, GError* error, gpointer data) {
    GtkWidget* status_bar = GTK_WIDGET(data);
    gtk_statusbar_pop(GTK_STATUSBAR(status_bar), 0);
}

// Callback for decide-destination signal - shows file chooser dialog
static gboolean decide_destination_cb(WebKitDownload* download, gchar* suggested_filename, gpointer data) {
    GtkWidget* status_bar = GTK_WIDGET(data);
    GtkWidget* window = gtk_widget_get_toplevel(status_bar);
    
    // Create file chooser dialog in save mode
    GtkFileChooserNative* chooser = gtk_file_chooser_native_new("Save File", GTK_WINDOW(window),
                                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
                                                                 "_Save", "_Cancel");
    
    // Set suggested filename
    if (suggested_filename) {
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(chooser), suggested_filename);
    }
    
    // Show dialog and get response
    gint response = gtk_native_dialog_run(GTK_NATIVE_DIALOG(chooser));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // User selected a file - set download destination
        gchar* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
        gchar* uri = g_filename_to_uri(filename, NULL, NULL);
        webkit_download_set_destination(download, uri);
        g_free(uri);
        g_free(filename);
        
        // Connect to progress signals for status bar updates
        g_signal_connect(download, "notify::estimated-progress", G_CALLBACK(update_download_progress), status_bar);
        g_signal_connect(download, "finished", G_CALLBACK(download_finished_cb), status_bar);
        g_signal_connect(download, "failed", G_CALLBACK(download_failed_cb), status_bar);
        
        g_object_unref(chooser);
        return TRUE;
    } else {
        // User cancelled - cancel the download
        g_object_unref(chooser);
        webkit_download_cancel(download);
        return FALSE;
    }
}

// Callback for download-started signal - connects decide-destination handler
static void download_started_cb(WebKitWebContext* context, WebKitDownload* download, gpointer data) {
    // Connect to decide-destination signal to show file chooser
    g_signal_connect(download, "decide-destination", G_CALLBACK(decide_destination_cb), data);
}

int main(int argc, char* argv[]) {
    // Initialize GTK library
    gtk_init(&argc, &argv);

    // Create WebKitWebContext and set a custom user-agent before any load
    WebKitWebContext* context = webkit_web_context_new();
    
    // Create a new WebKit WebView with the configured context
    WebKitWebView* webview = WEBKIT_WEB_VIEW(webkit_web_view_new_with_context(context));

    // Create a new top-level window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    // Set window title
    gtk_window_set_title(GTK_WINDOW(window), "🌿 Leaf Chat 🌿");
    
    // Set window icon from app.png (silently fails if file is missing)
    gtk_window_set_icon_from_file(GTK_WINDOW(window), "app.png", NULL);
    
    // Set window default size to 1920x1080
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);
    
    // Connect destroy signal to quit the application
    g_signal_connect(window, "destroy", G_CALLBACK(destroy_window_cb), NULL);
    
    // Create vertical box container for webview and status bar
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Add WebView to the container
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(webview), TRUE, TRUE, 0);
    
    // Create status bar for download progress
    GtkWidget* status_bar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox), status_bar, FALSE, FALSE, 0);
    
    // Connect download-started signal to handle file downloads
    g_signal_connect(context, "download-started", G_CALLBACK(download_started_cb), status_bar);
    
    // Load WhatsApp Web URL
    webkit_web_view_load_uri(webview, "https://web.whatsapp.com");
    
    // Show all widgets in the window
    gtk_widget_show_all(window);

    // Start GTK main event loop
    gtk_main();
    
    return 0;
}
