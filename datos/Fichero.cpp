#include "Fichero.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

Fichero::Fichero(void){
    urlPadre = "";
    contadorUrl = 0;
}

int Fichero::getContador(void){
    return contadorUrl;
}

void Fichero::setContador(int cont){
    contadorUrl = cont;
}

void Fichero::setPadre(string padre){
    urlPadre = padre;
}

string Fichero::getPadre(void){
    return urlPadre;
}

bool Fichero::leerContador(void){
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

bool Fichero::guardarContador(void){
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

bool Fichero::leerPadre(void){
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

bool Fichero::guardarPadre(void){
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