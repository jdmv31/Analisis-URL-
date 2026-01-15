#pragma once
#include <string>

using std::string;

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
};