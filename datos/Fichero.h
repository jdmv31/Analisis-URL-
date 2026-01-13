#pragma once
#include <string>

class Fichero{
    private:
        int contadorUrl;
        std::string urlPadre;
    public:
        Fichero(void);
        int getContador(void);
        void setContador(int cont);
        std::string getPadre(void);
        void setPadre(std::string padre);
        bool leerContador(void);
        bool guardarContador(void);
        bool leerPadre(void);
        bool guardarPadre(void);
};