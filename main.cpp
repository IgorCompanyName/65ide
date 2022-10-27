#include "mos6502.h"
#include <iostream>
#include <gtk/gtk.h>
#include <string>
#include <sstream>

#define ORGNAME "migstudio"
#define APPGROUP "65ide"
#define APPNAME "emu"

uint8_t* ram;

uint8_t readFunc(uint16_t addr) {
    return ram[addr];
}
void writeFunc(uint16_t addr, uint8_t data) {
    ram[addr] = addr;
}

static void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello, world!\n");
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *button_box;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK window");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window), button_box);

    button = gtk_button_new_with_label("Hello, world!");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_container_add(GTK_CONTAINER(button_box), button);

    gtk_widget_show_all(window);
}

static std::string getAppId() {
    std::stringstream ss;

    ss << ORGNAME << '.';
    ss << APPGROUP << '.';
    ss << APPNAME;

    return ss.str();
}

int main(int argc, char** argv) {
    GtkApplication *app;
    int status;

    std::string appid_tmp = getAppId();
    char* appid = (char*)malloc((appid_tmp.size() + 1) * sizeof(char));
    strcpy(appid, appid_tmp.c_str());

    app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return 0;
}