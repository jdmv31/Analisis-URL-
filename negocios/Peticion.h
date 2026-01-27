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
        int limitePaginas;
        int nivelProfundidad;
        bool usarProfundidad;
        int paginasHuerfanas;
        float promedioLinks;
        int cantImagenes;
        Cola colaPrioridad;
        Fichero gestorFicheros;
    public:
        Peticion();
        void reset();
        void configurar(bool profundidad, int limite);
        int realizarPeticion(string url);
        int getCantIMG();
        int getPromedioLinks();
        int getPaginasHuerfanas();
        int getCantNodos();
        void parsearHtml (string html,string urlPadre, int nivel);
        void extraerEtiquetas(GumboNode* nodo,vector<string>& urlsRecolectadas);
        void guardarInformacion();
        void leerInformacion();
        void procesarLinks(vector<string> urlsRecolectadas, string urlPadre, int nivel);
        bool buscarPalabra(string palabraClave);
        void calcularMetricas();
        int contarImagenes(GumboNode* nodo);
        int contarLinks(GumboNode* nodo);
        bool datosCola();
        string obtenerPadre(string urlHija);
        /*
            metricas estructurales: 
            cantidad de paginas huerfanas: las que no se puede acceder
            cantidad de nodos: la cantidad de paginas
            cantidad de imagenes: cantidad de imagenes
            promedio de enlaces en la pagina : cantidad de links / cantidad de paginas
        */
};