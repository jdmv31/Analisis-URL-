#include <presentacion/Interfaz.h>
#include <datos/Fichero.h>
#include <negocios/Peticion.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[]) {
    /*
    string paginaweb = "https://uneg.edu.ve";
    Peticion peticion;
    cout<<peticion.realizarPeticion(paginaweb)<<endl;
    peticion.guardarInformacion();
    peticion.destructor();
    */

    auto app = Gtk::Application::create("com.miwebcrawler.app");
    return app->make_window_and_run<MiVentana>(argc, argv);
}
