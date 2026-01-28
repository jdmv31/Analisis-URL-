#pragma once
#include <string>
#include <negocios/Cola.h>

using std::string;


struct Metricas{
    int paginasHuerfanas;
    int cantImagenes;
    float promedioLinks;
    int totalPaginas;
};

class Fichero{
    private:
        int contadorUrl;
        std::string urlPadre;
    public:
        Fichero(void);
        int getContador();
        void setContador(int cont);
        std::string getPadre();
        void setPadre(std::string padre);
        bool leerContador();
        bool guardarContador();
        bool leerPadre();
        bool guardarPadre();
        bool guardarMetricas(Metricas metricas);
        bool leerMetricas(Metricas &metricas);
        bool guardarCola(Cola &colaPrioridad);
        bool leerCola(Cola &colaPrioridad);
};