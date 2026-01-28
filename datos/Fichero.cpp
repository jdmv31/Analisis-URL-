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

// Declaración del constructor de la clase Fichero
Fichero::Fichero(){
    urlPadre = "";
    contadorUrl = 0;
}
// getter que obtiene la información de la variable contadorUrl
int Fichero::getContador(){
    return contadorUrl;
}

// setter que obtiene la información de la capa de negocios para asignarle un valor a la variable contadorUrl
void Fichero::setContador(int cont){
    contadorUrl = cont;
}
// setter que obtiene la información de la capa de negocios para asignarle un valor a la variable urlPadre
void Fichero::setPadre(string padre){
    urlPadre = padre;
}
// getter que obtiene la información de la variable urlPadre
string Fichero::getPadre(){
    return urlPadre;
}
// Obtiene la información del contador de urls desde el archivo de texto
bool Fichero::leerContador(){
    int num = 0;
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorUrl";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta/"contador.txt";
    // Apertura del archivo de texto para la lectura
    std::ifstream archivo(rutaArchivo);
    // Lectura del valor del contador desde el archivo de texto y guardado de la información en la variable
    if (archivo.is_open()){
        archivo >> num;
        contadorUrl = num;
        archivo.close();
        return true;
    }
    else
        return false;
}
// Guarda la información del contador de urls en un archivo de texto
bool Fichero::guardarContador(){
    int num = 0;
    fs::path rutaBase (RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorUrl";
    // si no existe la carpeta, se crea
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
        // declara el path del archivo para el guardado
    fs::path rutaArchivo = carpeta / "contador.txt";
    std::ofstream archivo (rutaArchivo);
    // escribe el valor del contador en el archivo de texto
    if (archivo.is_open()){
        num = getContador();
        archivo << num;
        archivo.close();
        return true;
    }   
    else
        return false;
}

// Obtiene la información de la url semilla desde un archivo de texto
bool Fichero::leerPadre(){
    string padre = "";
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "urlPadre";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);

    fs::path rutaArchivo = carpeta / "urlPadre.txt";
    std::ifstream archivo (rutaArchivo);
    // Busca la información de la url para guardarla en la variable urlPadre
    if (archivo.is_open()){
        archivo>>padre;
        urlPadre = padre;
        archivo.close();
        return true;
    }
    else
        return false;    
}
// Guarda la información de la url semilla en un archivo de texto
bool Fichero::guardarPadre(){
    string padre = "";
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "urlPadre";
    // creación de la carpeta si no existe
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
// apertura del archivo de texto para el guardado
    fs::path rutaArchivo = carpeta / "urlPadre.txt";
    std::ofstream archivo (rutaArchivo);
    // escritura de la url semilla en el archivo
    if (archivo.is_open()){
        padre = getPadre();
        archivo<<padre;
        archivo.close();
        return true;
    }
    else
        return false;
}

// transforma la información de la cola en bytes para guardarlo en un archivo binario
bool Fichero::guardarCola(Cola &colaPrioridad){
    vector<vectorAux> aux;
    colaPrioridad.copiarDatos(aux);
// declaración de rutas y creación de carpetas si no existen
    fs::path rutaBase(RUTA_DATOS);
    fs::path rutaCarpeta = rutaBase / "urlAlmacenadas";
    if (!fs::exists(rutaCarpeta))
        fs::create_directories(rutaCarpeta);
// apertura del archivo binario 
    fs::path rutaArchivo = rutaCarpeta / ("colaAlmacenada.bin");
    std::ofstream archivo (rutaArchivo, std::ios::binary | std::ios::trunc);
    if (!archivo.is_open()) return false;
    size_t elementos = aux.size();
    archivo.write(reinterpret_cast<char*>(&elementos),sizeof(size_t));
// escritura de los datos de la cola en el archivo
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

// Reconstruye la cola desde el archivo binario creado
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
    // lectura de los datos y reconstrucción de la cola
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

//  función que guarda la información de las métricas en un archivo de texto 
bool Fichero::guardarMetricas(Metricas metricas){
    // declaración de rutas para el guardado del archivo
fs::path rutaBase(RUTA_DATOS);

    fs::path carpeta = rutaBase / "contadorUrl";
    
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);

    fs::path rutaArchivo = carpeta / "metricasGuardadas.txt";
    std::ofstream archivo(rutaArchivo);

    // escritura de las métricas en el archivo de texto
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
// Función encargada de leer el archivo de texto de métricas para extraer su información
bool Fichero::leerMetricas(Metricas &metricas) {
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorUrl";
    fs::path rutaArchivo = carpeta / "metricasGuardadas.txt";

// retorna false en caso de que no exista el archivo
    if (!fs::exists(rutaArchivo)) return false;

    std::ifstream archivo(rutaArchivo);
    // lectura de las métricas desde el archivo de texto
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