#pragma once
#include <string>
#include <vector>

using std::vector;
using std::string;

struct vectorAux{
    string url;
    int prioridad;
};

struct Nodo{
    string url;
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
        int getLongitud();
        void insertarUrl(string url, int prioridad);
        Nodo* crearNodo(string url, int prioridad);
        void copiarDatos(vector<vectorAux>& lista);
        void imprimirCola();
};