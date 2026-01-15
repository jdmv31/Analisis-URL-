#pragma once
#include <string>
#include <gumbo.h>
#include <negocios/Cola.h>
#include <datos/Fichero.h>
#include <vector>

using std::vector;
using namespace std;

class Peticion{
    private:
        string urlSolicitada;
        const int MAX_PAGINAS = 25;
        Cola colaPrioridad;
        Fichero gestorFicheros;
    public:
        Peticion();
        int realizarPeticion(string url);
        void parsearHtml (string html);
        void extraerEtiquetas(GumboNode* nodo,vector<string>& urlsRecolectadas);
        void guardarInformacion();
        int obtenerEtiquetasTexto(GumboNode* nodo);
        void procesarLinks(vector<string> urlsRecolectadas);
};