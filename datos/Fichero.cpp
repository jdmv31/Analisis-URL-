#include "Fichero.h"
#include "PathConfig.h"
#include <filesystem>
#include <fstream>
#include <negocios/Cola.h>
#include <iostream>
#include <vector>

using std::vector;
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

bool Fichero::guardarCola(Cola &colaPrioridad){
    vector<vectorAux> aux;
    colaPrioridad.copiarDatos(aux);

    fs::path rutaBase(RUTA_DATOS);
    fs::path rutaCarpeta = rutaBase / "urlAlmacenadas";
    if (!fs::exists(rutaCarpeta))
        fs::create_directories(rutaCarpeta);

    fs::path rutaArchivo = rutaCarpeta / ("colaAlmacenada.bin");
    std::ofstream archivo (rutaArchivo, std::ios::binary | std::ios::trunc);
    if (!archivo.is_open()) return false;
    size_t elementos = aux.size();
    archivo.write(reinterpret_cast<char*>(&elementos),sizeof(size_t));

    for (auto& item: aux){
        archivo.write(reinterpret_cast<char*>(&item.prioridad), sizeof(int));

        size_t tamUrl = item.url.size();
        archivo.write(reinterpret_cast<char*>(&tamUrl), sizeof(size_t));
        archivo.write(item.url.c_str(), tamUrl);

        size_t tamPadre = item.urlPadre.size();
        archivo.write(reinterpret_cast<char*>(&tamPadre), sizeof(size_t));
        archivo.write(item.urlPadre.c_str(), tamPadre);

        archivo.write(reinterpret_cast<char*>(&item.nivel), sizeof(int));
    }
    archivo.close();
    return true;
}


bool Fichero::leerCola(Cola &colaPrioridad){

    fs::path rutaBase(RUTA_DATOS);
    fs::path rutaCarpeta = rutaBase / "urlAlmacenadas";
    if (!fs::exists(rutaCarpeta))
        fs::create_directories(rutaCarpeta);

    fs::path rutaArchivo = rutaCarpeta / ("colaAlmacenada.bin");
    std::ifstream archivo (rutaArchivo, std::ios::binary);
    if (!archivo.is_open()) return false;
    
    size_t elementos = 0;
    archivo.read(reinterpret_cast<char*>(&elementos),sizeof(size_t));
    for (int i = 0; i < elementos; i++){
        vectorAux item;
        archivo.read(reinterpret_cast<char*>(&item.prioridad), sizeof(int));

        size_t tamUrl = 0;
        archivo.read(reinterpret_cast<char*>(&tamUrl), sizeof(size_t));
        item.url.resize(tamUrl);
        archivo.read(&item.url[0], tamUrl);

        size_t tamPadre = 0;
        archivo.read(reinterpret_cast<char*>(&tamPadre), sizeof(size_t));
        item.urlPadre.resize(tamPadre);
        archivo.read(&item.urlPadre[0], tamPadre);

        archivo.read(reinterpret_cast<char*>(&item.nivel), sizeof(int));

        colaPrioridad.insertarUrl(item.url, item.prioridad, item.urlPadre, item.nivel);
    }
    return true;
}

bool Fichero::guardarMetricas(Metricas metricas){
fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorUrl";
    
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);

    fs::path rutaArchivo = carpeta / "metricasGuardadas.txt";
    std::ofstream archivo(rutaArchivo);

    if (archivo.is_open()) {
        archivo << metricas.paginasHuerfanas << "\n";
        archivo << metricas.cantImagenes << "\n";
        archivo << metricas.promedioLinks << "\n";
        archivo << metricas.totalPaginas << "\n";
        
        archivo.close();
        return true;
    }
        
    return false; 
}

bool Fichero::leerMetricas(Metricas &metricas) {
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorUrl";
    fs::path rutaArchivo = carpeta / "metricasGuardadas.txt";

    if (!fs::exists(rutaArchivo)) return false;

    std::ifstream archivo(rutaArchivo);
    if (archivo.is_open()) {
        archivo >> metricas.paginasHuerfanas;
        archivo >> metricas.cantImagenes;
        archivo >> metricas.promedioLinks;
        archivo >> metricas.totalPaginas;
        
        archivo.close();
        return true;
    }
        return false;
}