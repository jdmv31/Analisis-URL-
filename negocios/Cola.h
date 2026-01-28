#pragma once
#include <string>
#include <vector>
#include <functional>

using std::vector;
using std::string;

// Auxiliar facilitador para el traslado de datos a la capa de permanecia (fichero)
struct vectorAux{
    string url;
    string urlPadre;
    int nivel;
    int prioridad;
};

//Figura cada pagina en la memoria y incluye un puntero que forma la lista enlazada en serie
struct Nodo{
    string url;
    string urlPadre;
    int nivel;
    int prioridad;
    Nodo* siguiente;
};

// Lista enlazada simplemente enlazada, responde como cola de prioridad
class Cola{
    private:
        Nodo* frente;
        int longitud;
    public:
        Cola();
        ~Cola(); // destructor de la clase
        void vaciarCola();
        int getLongitud();
        void insertarUrl(string url, int prioridad, string urlPadre, int nivel);
        Nodo* crearNodo(string url, int prioridad, string urlPadre, int nivel);
        void copiarDatos(vector<vectorAux>& lista);
        void recorrerCola(std::function<bool(string, int, string, int)> accion);// se usa un callback para procesar datos sin exponer la estructura interna 
        bool colaVacia();
};