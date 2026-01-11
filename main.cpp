#include <presentacion/Interfaz.h>
#include <datos/Ficheros.cpp>
#include <negocios/Peticion.cpp>
#include <iostream>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.ejemplo.gtkmm");

    // Ejecutar la aplicación mostrando la ventana
    return app->make_window_and_run<MiVentana>(argc, argv);
}