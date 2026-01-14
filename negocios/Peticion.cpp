#include "Peticion.h"
#include <cpr/cpr.h>
#include <gumbo.h>
#include <string>
#include "datos/Fichero.h"
#include <iostream>

using std::string;
using std::endl;

Peticion::Peticion(void){
    urlSolicitada = "";
    // c++ automaticamente usa el constructor del objeto cola, no es necesario especificar
}

void Peticion::destructor(void){
    colaPrioridad.destructor();
    urlSolicitada = "";
}

int Peticion::realizarPeticion(string url) {
    auto r = cpr::Get(
        cpr::Url{static_cast<string_view>(url)},
        cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
        cpr::Parameters{{"anon", "true"}, {"key", "value"}}
    );

    if (r.status_code == 0)
        return 0;

    if (r.status_code >= 400)
        return 400;

    string html = static_cast<string>(r.text);
    urlSolicitada = url;
    parsearHtml(html);
    return 200;
}

void Peticion::parsearHtml (string html){
    GumboOutput* salida = gumbo_parse(html.c_str());
    extraerEtiquetas(salida->root);
    gumbo_destroy_output(&kGumboDefaultOptions, salida);
}

void Peticion::extraerEtiquetas(GumboNode* nodo){
    if (nodo->type != GUMBO_NODE_ELEMENT)
        return;

    int cantEtiquetas = 0;

    GumboAttribute* href;
    if (nodo->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&nodo->v.element.attributes,"href"))){
            if (!static_cast<string>(href->value).find("http") && colaPrioridad.getLongitud() < MAX_PAGINAS){
                // aca las etiquetas
                int etiquetas = obtenerEtiquetasTexto(nodo);
                colaPrioridad.insertarUrl(static_cast<string>(href->value));
                std::cout<<"etiquetas de texto: "<< etiquetas<<std::endl;
            }
    }
    GumboVector* hijos = &nodo->v.element.children;

    for (int i =0; i < hijos->length;i++){
        extraerEtiquetas(static_cast<GumboNode*>(hijos->data[i]));
    }
}


int Peticion::obtenerEtiquetasTexto(GumboNode* nodo){
    if (nodo->type != GUMBO_NODE_ELEMENT)
        return 0;

    int cuentaActual = 0;
    if (nodo->v.element.tag == GUMBO_TAG_P || nodo->v.element.tag == GUMBO_TAG_H1
        || nodo->v.element.tag == GUMBO_TAG_H2 || nodo->v.element.tag == GUMBO_TAG_H3
        || nodo->v.element.tag == GUMBO_TAG_H4 || nodo->v.element.tag == GUMBO_TAG_H5
        || nodo->v.element.tag == GUMBO_TAG_H6) 
        cuentaActual = 1;

    GumboVector* hijos = &nodo->v.element.children;
    int sumaHijos = 0;
    
    for (int i = 0; i < hijos->length; i++){
        sumaHijos += obtenerEtiquetasTexto(static_cast<GumboNode*>(hijos->data[i]));
    }

    return cuentaActual + sumaHijos;
}

void Peticion::guardarInformacion(void){
    gestorFicheros.setPadre(urlSolicitada);
    int elementos = colaPrioridad.getLongitud();
    gestorFicheros.setContador(elementos);
}