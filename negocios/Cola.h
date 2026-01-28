#pragma once
#include <string>
#include <vector>
#include <functional>

using std::vector;
using std::string;

struct vectorAux{
    string url;
    string urlPadre;
    int nivel;
    int prioridad;
};

struct Nodo{
    string url;
    string urlPadre;
    int nivel;
    int prioridad;
    Nodo* siguiente;
};

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
        void recorrerCola(std::function<bool(string, int, string, int)> accion);
        bool colaVacia();
};