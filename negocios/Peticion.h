#pragma once
#include <string>
#include <gumbo.h>
#include <negocios/Cola.h>
#include <datos/Fichero.h>
#include <vector>
#include <sstream>
#include <set>

using std::set;
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
        int totalEnlaces;
        Cola colaPrioridad;
        Fichero gestorFicheros;
        string rutaEncontrada;
        set<string> paginasVisitadas;
    public:
        Peticion();
        string obtenerListado();
        void reset();
        void configurar(bool profundidad, int limite);
        int realizarPeticion(string url);
        void setLimitePaginas(int dato);
        int getCantIMG();
        float getPromedioLinks();
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
        string obtenerRuta();
        string extraerDominio(string url);
        void procesarUrl(string url, int nivelActual);
        void procesarUrlRecursivo(string url, int nivelActual);
        /*
            metricas estructurales: 
            cantidad de paginas huerfanas: las que no se puede acceder
            cantidad de nodos: la cantidad de paginas
            cantidad de imagenes: cantidad de imagenes
            promedio de enlaces por pagina : cantidad de links / cantidad de paginas
        */
};