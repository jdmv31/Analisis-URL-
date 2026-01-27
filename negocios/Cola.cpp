#include "negocios/Cola.h"
#include "datos/Fichero.h"
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

Nodo* Cola::crearNodo(string url, int prioridad, string urlPadre, int nivel){
    Nodo* nuevonodo;
    nuevonodo = new Nodo();
    nuevonodo->url = url;
    nuevonodo->prioridad = prioridad;
    nuevonodo->urlPadre = urlPadre;
    nuevonodo->nivel = nivel;
    nuevonodo->siguiente = nullptr;
    return nuevonodo;
}

void Cola::insertarUrl(string url, int prioridad, string urlPadre, int nivel){
    Nodo* nuevonodo = crearNodo(url,prioridad,urlPadre,nivel);

    if (frente == nullptr || prioridad > frente->prioridad){
        nuevonodo->siguiente = frente;
        frente = nuevonodo;
    }
    else{
        Nodo* actual = frente;
        while (actual->siguiente != nullptr && actual->siguiente->prioridad >= prioridad){
            actual = actual->siguiente;
        }
        
        nuevonodo->siguiente = actual->siguiente;
        actual->siguiente = nuevonodo;
    }

    longitud++;
}

void Cola::copiarDatos(vector<vectorAux>& lista){
    Nodo *nuevonodo = frente;
    while (nuevonodo != nullptr){
        lista.push_back({nuevonodo->url, nuevonodo->urlPadre, nuevonodo->nivel, nuevonodo->prioridad});
        nuevonodo = nuevonodo->siguiente;
    }
}

void Cola::recorrerCola(std::function<bool(string,int,string,int)> accion){
    Nodo* actual = frente;
    while (actual != nullptr){
        if (accion(actual->url, actual->prioridad,actual->urlPadre,actual->nivel))
            return;         
        actual = actual->siguiente;
    }
}


bool Cola::colaVacia(){
    if (frente == nullptr)
        return true;
    return false;
}