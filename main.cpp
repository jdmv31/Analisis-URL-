#include <presentacion/Interfaz.h>
#include <datos/Fichero.h>
#include <negocios/Peticion.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("com.miwebcrawler.app");
    return app->make_window_and_run<Interfaz>(argc, argv);
}
