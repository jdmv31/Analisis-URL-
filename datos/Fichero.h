#pragma once
#ifndef RUTA_DATOS
#define RUTA_DATOS "./"
#endif
#include <string>

using std::string;

class Fichero{
    private:
        int contadorUrl;
        string urlPadre;
    public:
        Fichero(void);
        int getContador(void);
        void setContador(int url);
        string getPadre(void);
        void setPadre(string padre);
        bool leerContador(void);
        bool guardarContador(void);
        bool leerPadre(void);
        bool guardarPadre(void);
};