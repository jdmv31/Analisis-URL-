#pragma once
#include <string>
#include <gumbo.h>
using namespace std;

class Peticion{
    private:
        string urlSolicitada;
        const int MAX_PAGINAS = 25;
    public:
        Peticion(void);
        string realizarPeticion(string url);
        void parsearHtml (string html);
        void extraerEtiquetas(GumboNode* nodo);
};