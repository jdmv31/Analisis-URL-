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

    GumboAttribute* href;
    if (nodo->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&nodo->v.element.attributes,"href"))){
            if (!static_cast<string>(href->value).find("http") && colaPrioridad.getLongitud() < MAX_PAGINAS)
                colaPrioridad.insertarUrl(static_cast<string>(href->value));
    }
    GumboVector* hijos = &nodo->v.element.children;

    for (int i =0; i < hijos->length;i++){
        extraerEtiquetas(static_cast<GumboNode*>(hijos->data[i]));
    }
}


int Peticion::obtenerEtiquetasTexto(string html){
    int cantEtiquetas;

    return cantEtiquetas;
}

void Peticion::guardarInformacion(void){
    gestorFicheros.setPadre(urlSolicitada);
    int elementos = colaPrioridad.getLongitud();
    gestorFicheros.setContador(elementos);
    if (gestorFicheros.guardarPadre() && gestorFicheros.guardarContador())
        std::cout<<"bien"<<endl;
}