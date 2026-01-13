#include <presentacion/Interfaz.h>
#include <datos/Fichero.cpp>
#include <negocios/Peticion.cpp>
#include <iostream>
#include <string>

/*using std::cout;
using std::endl;
using std::cin;
using std::string;*/

int main(int argc, char* argv[]) {
    /*string paginaweb;
    cout<<"ingrese el link de su pagina web"<<endl;
    cin>>paginaweb;
    Peticion peticion;
    cout<<peticion.realizarPeticion(paginaweb)<<endl;

    peticion.destructor();
    return 0;*/

    auto app = Gtk::Application::create("com.miwebcrawler.app");
    return app->make_window_and_run<MiVentana>(argc, argv);
}
