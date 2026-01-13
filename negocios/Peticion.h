#pragma once
#include <string>
#include <gumbo.h>
#include <negocios/Cola.h>

using namespace std;

class Peticion{
    private:
        string urlSolicitada;
        const int MAX_PAGINAS = 25;
        Cola colaPrioridad;
    public:
        Peticion(void);
        void destructor(void);
        string realizarPeticion(string url);
        void parsearHtml (string html);
        void extraerEtiquetas(GumboNode* nodo);
};