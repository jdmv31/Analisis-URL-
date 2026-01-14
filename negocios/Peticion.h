#pragma once
#include <string>
#include <gumbo.h>
#include <negocios/Cola.h>
#include <datos/Fichero.h>

using namespace std;

class Peticion{
    private:
        string urlSolicitada;
        const int MAX_PAGINAS = 25;
        Cola colaPrioridad;
        Fichero gestorFicheros;
    public:
        Peticion(void);
        void destructor(void);
        int realizarPeticion(string url);
        void parsearHtml (string html);
        void extraerEtiquetas(GumboNode* nodo);
        void guardarInformacion(void);
        int obtenerEtiquetasTexto(string html);
};