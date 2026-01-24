#include "Peticion.h"
#include <cpr/cpr.h>
#include <gumbo.h>
#include <string>
#include "datos/Fichero.h"
#include <iostream>
#include <vector>
#include <future>

using std::string;
using std::endl;
using std::vector;


Peticion::Peticion(){
    urlSolicitada = "";
    paginasHuerfanas = 0;
    promedioLinks = 0;
    cantImagenes = 0;
    // c++ automaticamente usa el constructor del objeto cola, no es necesario especificar
}

int Peticion::getCantIMG(){
    return cantImagenes;
}

int Peticion::getPromedioLinks(){
    return promedioLinks;
}

int Peticion::getPaginasHuerfanas(){
    return paginasHuerfanas;
}

int Peticion::getCantNodos(){
    return colaPrioridad.getLongitud();
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
    vector<string> urlsRecolectadas;
    extraerEtiquetas(salida->root,urlsRecolectadas);
    procesarLinks(urlsRecolectadas);
    gumbo_destroy_output(&kGumboDefaultOptions, salida);
}

void Peticion::procesarLinks(vector<string> urlsRecolectadas){
    vector<cpr::AsyncResponse> links;
    links.reserve(urlsRecolectadas.size());
    for (string url : urlsRecolectadas){
        links.push_back(cpr::GetAsync(cpr::Url{url})); 
    }
    for (int i = 0; i < links.size(); i++) {
        cpr::Response r = links[i].get();
        if (r.status_code == 200) {
            GumboOutput* output = gumbo_parse(r.text.c_str());
            int cantidad = obtenerEtiquetasTexto(output->root);
            colaPrioridad.insertarUrl(urlsRecolectadas[i], cantidad);
            gumbo_destroy_output(&kGumboDefaultOptions, output);
        }
    }

}

void Peticion::extraerEtiquetas(GumboNode* nodo, vector<string>& urlsRecolectadas){
    int totalActual = colaPrioridad.getLongitud() + urlsRecolectadas.size();
    // va a buscar un total de 25 links, los primeros 25
    // si no tiene acceso a algun link, por ejemplo un forbidden o simplemente no se puede hacer nada
    // entonces no se toma en cuenta para la cola de prioridad, pueden haber menos de 25
    if (totalActual >= MAX_PAGINAS)
        return;

    if (nodo->type != GUMBO_NODE_ELEMENT)
        return;

    GumboAttribute* href;
    if (nodo->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&nodo->v.element.attributes,"href"))){
        if (colaPrioridad.getLongitud() + urlsRecolectadas.size() < MAX_PAGINAS) {
             string url = static_cast<string>(href->value);
             if (url.find("http") == 0)
                 urlsRecolectadas.push_back(url);
        }
    }

    GumboVector* hijos = &nodo->v.element.children;
    for (int i = 0; i < hijos->length; i++){
        if (colaPrioridad.getLongitud() + urlsRecolectadas.size() >= MAX_PAGINAS)
            break;
            
        extraerEtiquetas(static_cast<GumboNode*>(hijos->data[i]), urlsRecolectadas);
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

void Peticion::guardarInformacion(){
    gestorFicheros.setPadre(urlSolicitada);
    int elementos = colaPrioridad.getLongitud();
    gestorFicheros.setContador(elementos);
    if (gestorFicheros.guardarPadre() && gestorFicheros.guardarContador() && gestorFicheros.guardarCola(colaPrioridad))
        std::cout<<"bien"<<endl;
}   


void Peticion::leerInformacion(){
    gestorFicheros.leerCola(colaPrioridad);
}


bool Peticion::buscarPalabra(string palabraClave){
    bool algunEncontrado = false;

    colaPrioridad.recorrerCola([&](string url, int prioridad) -> bool {
        if (url.find(palabraClave) != std::string::npos) {
            std::cout << "\n=== ¡COINCIDENCIA EN LA URL! ===" << std::endl;
            std::cout << "Ruta de navegacion para llegar al objetivo:" << std::endl;
            std::cout << "1. Ingresar a: " << gestorFicheros.getPadre() << std::endl;
            std::cout << "2. Clickear en:  " << url << std::endl;                  
            
            std::cout << "\n(Prioridad del enlace: " << prioridad << ")" << std::endl;
            
            algunEncontrado = true;
            return true;
        }
        
        return false; 
    });

    return algunEncontrado;
}


int Peticion::contarImagenes(GumboNode* nodo){
    if (nodo->type != GUMBO_NODE_ELEMENT) 
        return 0;

    int cont = (nodo->v.element.tag == GUMBO_TAG_IMG) ? 1 : 0;

    GumboVector* hijos = &nodo->v.element.children;
    for (int i = 0; i < hijos->length; ++i) {
        cont += contarImagenes(static_cast<GumboNode*>(hijos->data[i]));
    }
    return cont;
}


int Peticion::contarLinks(GumboNode* nodo){
    if (nodo->type != GUMBO_NODE_ELEMENT) 
        return 0;

    int cont = (nodo->v.element.tag == GUMBO_TAG_A) ? 1 : 0;
    
    GumboVector* hijos = &nodo->v.element.children;
    for (int i = 0; i < hijos->length; ++i) {
        cont += contarLinks(static_cast<GumboNode*>(hijos->data[i]));
    }
    return cont; 
}   


void Peticion::calcularMetricas(){
    cantImagenes = 0;
    paginasHuerfanas = 0;
    int totalLinks = 0;
    promedioLinks = 0;
    colaPrioridad.recorrerCola([&](string url, int prioridad) -> bool {
        cpr::Response r = cpr::Get(cpr::Url{url});

        if (r.status_code == 404 || r.status_code == 403 || r.status_code == 0)
            paginasHuerfanas++;
        
        if (r.status_code == 200){
            GumboOutput* salida = gumbo_parse(r.text.c_str());  
            cantImagenes += contarImagenes(salida->root);
            totalLinks += contarLinks(salida->root);
            gumbo_destroy_output(&kGumboDefaultOptions, salida);
        }
        return false;
    });

    promedioLinks = (float) totalLinks / (colaPrioridad.getLongitud());

    std::cout << "\n--- Reporte de Metricas ---" << std::endl;
    std::cout << "Paginas Huerfanas: " << paginasHuerfanas << std::endl;
    std::cout << "Cantidad de Imagenes: " << cantImagenes << std::endl;
    std::cout << "Promedio de Enlaces: " << promedioLinks << std::endl;
    std::cout << "Cantidad de paginas visitadas: "<<colaPrioridad.getLongitud()<<std::endl;
}


bool Peticion::datosCola(){
    return colaPrioridad.colaVacia();
}