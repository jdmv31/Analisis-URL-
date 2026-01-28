#pragma once
#include <string>
#include <negocios/Cola.h>

using std::string;

// Definición de la estructura para almacenar las métricas del análisis
struct Metricas{
    int paginasHuerfanas;
    int cantImagenes;
    float promedioLinks;
    int totalPaginas;
};

// Declaración de la Clase 
class Fichero{
    private:
        int contadorUrl;
        std::string urlPadre;
    public:
        Fichero(void); // constructor de la clase fichero
        
        // Gestión de atributos de la clase
        int getContador(); 
        void setContador(int cont);
        std::string getPadre();
        void setPadre(std::string padre);

        // Guardado y lectura de datos para el contador y el padre
        bool leerContador(); // Permite cargar la cantidad almacenada de Urls desde el disco
        bool guardarContador(); // Guarda la cantidad de Urls en el disco
        bool leerPadre(); // Carga desde el disco la Url semilla (la Url Padre) desde el disco
        bool guardarPadre(); // Permite guardar la Url Padre en disco

        // Guardado y lectura de datos para la cola y las métricas
        bool guardarMetricas(Metricas metricas); // Permite guardar las metricas en disco
        bool leerMetricas(Metricas &metricas);// Lee en el disco los datos guardados de las metricas
        bool guardarCola(Cola &colaPrioridad); // Guarda en disco la información de la Cola 
        bool leerCola(Cola &colaPrioridad); // Permite leer la información de la Cola desde el disco
};