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
    colaPrioridad.insertarUrl(url,url.length(),"Inicio",0);
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
    parsearHtml(html,url,0);
    return 200;
}

void Peticion::parsearHtml (string html,string urlPadre, int nivel){
    GumboOutput* salida = gumbo_parse(html.c_str());
    vector<string> urlsRecolectadas;
    extraerEtiquetas(salida->root,urlsRecolectadas);
    procesarLinks(urlsRecolectadas,urlPadre,nivel);
    gumbo_destroy_output(&kGumboDefaultOptions, salida);
}

void Peticion::procesarLinks(vector<string> urlsRecolectadas, string urlPadre, int nivel){
    for (const string& url : urlsRecolectadas){
        int prioridad = url.length();
        colaPrioridad.insertarUrl(url, prioridad, urlPadre,nivel);
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

string Peticion::obtenerPadre(string urlHija){
    string urlPadre = "";

    colaPrioridad.recorrerCola([&](string url, int p, string padre, int n) -> bool {
        if (url == urlHija) {
            urlPadre = padre;
            return true; 
        }
        return false; 
    });
    return urlPadre;
}


bool Peticion::buscarPalabra(string palabraClave){
    bool algunEncontrado = false;

    colaPrioridad.recorrerCola([&](string url, int prioridad, string padre, int nivel) -> bool {
        if (url.find(palabraClave) != std::string::npos) {
            std::cout << "\n========================================" << std::endl;
            std::cout << "¡PALABRA ENCONTRADA EN NIVEL " << nivel << "!" << std::endl;
            std::cout << "URL: " << url << std::endl;
            
            vector<string> camino;
            string actual = url;
            string padreActual = padre;

            camino.push_back(actual);
            while (padreActual != "Inicio" && !padreActual.empty()) {
                camino.push_back(padreActual);
                actual = padreActual;
                padreActual = obtenerPadre(actual);
            }

            std::cout << "\n--- Ruta de Navegación (Backtracking) ---" << std::endl;
            std::cout << "Cantidad de Clicks necesarios: " << (camino.size() - 1) << std::endl;
            
            int paso = 0;
            for (int i = camino.size() - 1; i >= 0; i--) {
                if (i == camino.size() - 1) 
                    std::cout << "[Inicio] " << camino[i] << std::endl;
                else {
                    std::cout << "    | " << std::endl;
                    std::cout << "    L-> (Click " << ++paso << ") " << camino[i] << std::endl;
                }
            }
            std::cout << "========================================\n" << std::endl;
            
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
    colaPrioridad.recorrerCola([&](string url, int prioridad, string padre, int nivel) -> bool {
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