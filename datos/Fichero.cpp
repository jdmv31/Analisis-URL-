#include "Fichero.h"
#include "PathConfig.h"
#include <filesystem>
#include <fstream>
#include <negocios/Cola.h>

using std::string;
namespace fs = std::filesystem;

Fichero::Fichero(){
    urlPadre = "";
    contadorUrl = 0;
}

int Fichero::getContador(){
    return contadorUrl;
}

void Fichero::setContador(int cont){
    contadorUrl = cont;
}

void Fichero::setPadre(string padre){
    urlPadre = padre;
}

string Fichero::getPadre(){
    return urlPadre;
}

bool Fichero::leerContador(){
    int num = 0;
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorUrl";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta/"contador.txt";
    std::ifstream archivo(rutaArchivo);
    if (archivo.is_open()){
        archivo >> num;
        contadorUrl = num;
        archivo.close();
        return true;
    }
    else
        return false;
}

bool Fichero::guardarContador(){
    int num = 0;
    fs::path rutaBase (RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorUrl";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "contador.txt";
    std::ofstream archivo (rutaArchivo);
    if (archivo.is_open()){
        num = getContador();
        archivo << num;
        archivo.close();
        return true;
    }   
    else
        return false;
}

bool Fichero::leerPadre(){
    string padre = "";
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "urlPadre";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);

    fs::path rutaArchivo = carpeta / "urlPadre.txt";
    std::ifstream archivo (rutaArchivo);
    if (archivo.is_open()){
        archivo>>padre;
        urlPadre = padre;
        archivo.close();
        return true;
    }
    else
        return false;    
}

bool Fichero::guardarPadre(){
    string padre = "";
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "urlPadre";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);

    fs::path rutaArchivo = carpeta / "urlPadre.txt";
    std::ofstream archivo (rutaArchivo);
    if (archivo.is_open()){
        padre = getPadre();
        archivo<<padre;
        archivo.close();
        return true;
    }
    else
        return false;
}

bool Fichero::guardarCola(Cola colaPrioridad){
    /*
    fs::path rutaBase(RUTA_DATOS); 
    fs::path rutaCarpeta = rutaBase / "urlsAlmacenadas";
    if (!fs::exists(rutaCarpeta))
        fs::create_directories(rutaCarpeta);


    fs::path rutaArchivo = rutaCarpeta / ("colaAlmacenada.bin");
    std::ofstream archivo(rutaArchivo,std::ios::binary | std::ios::trunc);
    
    if (!archivo.is_open()) return false;

    Nodo* actual = colaPrioridad->frente;
    
    while (actual != nullptr) {
        int tamanoUrl = actual->url.size();
        archivo.write(reinterpret_cast<char*>(&tamanoUrl), sizeof(int));
        archivo.write(actual->url.c_str(), tamanoUrl);
        archivo.write(reinterpret_cast<char*>(&actual->prioridad), sizeof(int));
        actual = actual->siguiente;
    }

    archivo.close();
    return true;
    */
   return true;
}


bool Fichero::leerCola(Cola colaPrioridad){
    return true;

}