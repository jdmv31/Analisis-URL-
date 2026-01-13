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
        Nodo* actual;
        Nodo* fin;
        int longitud;
    public:
        Cola(void);
        void destructor(void);
        int getLongitud(void);
        void insertarUrl(string url);
};