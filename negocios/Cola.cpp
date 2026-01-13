#include "negocios/Cola.h"
#include <string>

using std::string;

Cola::Cola(void){
    frente = actual = fin = nullptr;
    longitud = 0;
}

void Cola::destructor(void){
    while (frente != nullptr){
        actual = frente;
        frente = frente->siguiente;
        delete actual;
    }
    actual = frente = fin = nullptr;
    longitud = 0;
}

int Cola::getLongitud(void){
    return longitud;
}

void Cola::insertarUrl(string url){
    Nodo* nuevonodo;
    nuevonodo = new Nodo();
    nuevonodo->url = url;
    nuevonodo->siguiente = nullptr;

    if (frente == nullptr){
        frente = nuevonodo;
        fin = nuevonodo;
    }
    else{
        fin->siguiente = nuevonodo;
        fin = nuevonodo;
    }

    longitud++;
}