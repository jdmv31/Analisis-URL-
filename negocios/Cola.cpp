#include "negocios/Cola.h"
#include <string>

using std::string;

Cola::Cola(){
    frente  = nullptr;
    longitud = 0;
}

Cola::~Cola(){
    while (frente != nullptr){
        Nodo* actual = frente;
        frente = frente->siguiente;
        delete actual;
    }
    frente = nullptr;
    longitud = 0;
}

int Cola::getLongitud(){
    return longitud;
}

Nodo* Cola::crearNodo(string url, int prioridad){
    Nodo* nuevonodo;
    nuevonodo = new Nodo();
    nuevonodo->url = url;
    nuevonodo->prioridad = prioridad;
    nuevonodo->siguiente = nullptr;
    return nuevonodo;
}

void Cola::insertarUrl(string url, int prioridad){
    Nodo* nuevonodo = crearNodo(url,prioridad);

    if (frente == nullptr || prioridad > frente->prioridad){
        nuevonodo->siguiente = frente;
        frente = nuevonodo;
    }
    else{
        Nodo* actual = frente;
        while (actual != nullptr && actual->siguiente->prioridad >= prioridad){
            actual = actual->siguiente;
        }
        nuevonodo->siguiente = actual->siguiente;
        actual->siguiente = nuevonodo;
    }

    longitud++;
}