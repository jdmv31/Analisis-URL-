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
        int paginasHuerfanas;
        float promedioLinks;
        int cantImagenes;
        Cola colaPrioridad;
        Fichero gestorFicheros;
    public:
        Peticion();
        int realizarPeticion(string url);
        void parsearHtml (string html);
        void extraerEtiquetas(GumboNode* nodo,vector<string>& urlsRecolectadas);
        void guardarInformacion();
        void leerInformacion();
        int obtenerEtiquetasTexto(GumboNode* nodo);
        void procesarLinks(vector<string> urlsRecolectadas);
        bool buscarPalabra(string palabraClave);
        bool buscarEnArbol(GumboNode* nodo,string palabra);
        void calcularMetricas();
        int contarImagenes(GumboNode* nodo);
        int contarLinks(GumboNode* nodo);
        bool datosCola();
        /*
            metricas estructurales: 
            cantidad de paginas huerfanas: las que no se puede acceder
            cantidad de nodos: la cantidad de paginas
            cantidad de imagenes: cantidad de imagenes
            promedio de enlaces en la pagina : cantidad de links / cantidad de paginas
        */
};