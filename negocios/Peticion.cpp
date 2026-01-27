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
    limitePaginas = 25;
    nivelProfundidad = 3;
    usarProfundidad = false;
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
    reset();
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

void Peticion::reset(){
    colaPrioridad.vaciarCola();
    urlSolicitada = "";
    paginasHuerfanas = 0;
    promedioLinks = 0;
    cantImagenes = 0;
}

void Peticion::parsearHtml (string html,string urlPadre, int nivel){
    GumboOutput* salida = gumbo_parse(html.c_str());
    vector<string> urlsRecolectadas;
    extraerEtiquetas(salida->root,urlsRecolectadas);
    procesarLinks(urlsRecolectadas,urlPadre,nivel);
    gumbo_destroy_output(&kGumboDefaultOptions, salida);
}

void Peticion::procesarLinks(vector<string> urlsRecolectadas, string urlPadre, int nivelPadre){
    int nuevoNivel = nivelPadre + 1;

    for (const string& url : urlsRecolectadas){
        bool insertar = false;

        if (usarProfundidad) {
            if (nuevoNivel <= nivelProfundidad)
                insertar = true;
        } else {
            if (colaPrioridad.getLongitud() < limitePaginas)
                insertar = true;
        }

        if (insertar) {
            int prioridad = url.length();
            colaPrioridad.insertarUrl(url, prioridad, urlPadre, nuevoNivel);
        }
    }
}

void Peticion::extraerEtiquetas(GumboNode* nodo, vector<string>& urlsRecolectadas){

    if (!usarProfundidad) {
        int totalActual = colaPrioridad.getLongitud() + urlsRecolectadas.size();
        if (totalActual >= limitePaginas) return;
    }

    if (nodo->type != GUMBO_NODE_ELEMENT) return;

    GumboAttribute* href;
    if (nodo->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&nodo->v.element.attributes,"href"))){
        if (!usarProfundidad)
             if (colaPrioridad.getLongitud() + urlsRecolectadas.size() >= limitePaginas) return;

        string url = static_cast<string>(href->value);
        if (url.find("http") == 0)
             urlsRecolectadas.push_back(url);
    }

    GumboVector* hijos = &nodo->v.element.children;
    for (int i = 0; i < hijos->length; i++){
        if (!usarProfundidad && (colaPrioridad.getLongitud() + urlsRecolectadas.size() >= limitePaginas)) 
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

void Peticion::configurar(bool profundidad, int limite){
    this->usarProfundidad = profundidad;
    if (profundidad)
        this->nivelProfundidad = limite;
    else
        this->limitePaginas = limite;
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
            std::cout << "\n=== ¡COINCIDENCIA ENCONTRADA! ===" << std::endl;
            std::cout << "URL: " << url << std::endl;
            std::cout << "Nivel de profundidad: " << nivel << std::endl;
            vector<string> camino;
            camino.push_back(url);
            string padreActual = padre;

            while (padreActual != "Inicio" && !padreActual.empty()) {
                camino.push_back(padreActual);

                string abuelo = "";
                colaPrioridad.recorrerCola([&](string u, int p, string pa, int n) -> bool {
                    if (u == padreActual) {
                        abuelo = pa;
                        return true;
                    }
                    return false;
                });
                padreActual = abuelo; 
            }

            std::cout << "\n--- Ruta de Clicks ---" << std::endl;
            std::cout << "Pasos necesarios: " << camino.size() << std::endl;
            
            for (int i = camino.size() - 1; i >= 0; i--) {
                std::cout << "[" << (camino.size() - 1 - i) << "] " << camino[i] << std::endl;
                if (i > 0) std::cout << "  |\n  V" << std::endl;
            }
            std::cout << "===============================\n" << std::endl;
            
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

//agregado por nicole
string Peticion::obtenerListado() {
    string resultado = "";
    int contador = 1;
    
    colaPrioridad.recorrerCola([&](string url, int prioridad, string padre, int nivel) -> bool {
        resultado += to_string(contador) + ". [Prio: " + to_string(prioridad) + "] " + url + "\n";
        contador++;
        return false; // Retornar false para seguir recorriendo
    });
    
    return resultado;
}


bool Peticion::datosCola(){
    return colaPrioridad.colaVacia();
}