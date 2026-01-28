#include "Peticion.h"
#include <cpr/cpr.h>
#include <gumbo.h>
#include <string>
#include "datos/Fichero.h"
#include <iostream>
#include <vector>
#include <future>
#include <sstream>

using std::string;
using std::endl;
using std::vector;


Peticion::Peticion(){
    urlSolicitada = "";
    paginasHuerfanas = 0;
    promedioLinks = 0;
    cantImagenes = 0;
    limitePaginas = 100;
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

string Peticion::extraerDominio(string url) {
    string dominio = url;

    size_t inicio = dominio.find("://");
    if (inicio != string::npos) {
        dominio = dominio.substr(inicio + 3);
    }

    if (dominio.find("www.") == 0) {
        dominio = dominio.substr(4);
    }

    size_t fin = dominio.find("/");
    if (fin != string::npos) {
        dominio = dominio.substr(0, fin);
    }

    return dominio;
}

int Peticion::getCantNodos(){
    return colaPrioridad.getLongitud();
}


void Peticion::procesarUrl(string url, int nivelActual) {
    if (usarProfundidad && nivelActual > nivelProfundidad) return;

    if (paginasVisitadas.find(url) != paginasVisitadas.end()) return;
    paginasVisitadas.insert(url);

    string padre = (nivelActual == 0) ? "Inicio" : obtenerPadre(url); 

    auto r = cpr::Get(
        cpr::Url{static_cast<string_view>(url)},
        cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
        cpr::Parameters{{"anon", "true"}, {"key", "value"}}
    );

    if (r.status_code == 200) {
        string html = static_cast<string>(r.text);

        if (nivelActual == 0) urlSolicitada = url;
        parsearHtml(html, url, nivelActual); 
    }
}


int Peticion::realizarPeticion(string url) {
    reset();
    paginasVisitadas.clear();

    if (url.find("http") != 0) {
        url = "http://" + url;
    }

    auto r = cpr::Get(
        cpr::Url{static_cast<string_view>(url)},
        cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
        cpr::Parameters{{"anon", "true"}, {"key", "value"}}
    );

    if (r.status_code == 0) return 0;
    if (r.status_code >= 400) return r.status_code; 

    urlSolicitada = url;
    paginasVisitadas.insert(url);
    
    colaPrioridad.insertarUrl(url, url.length(), "Inicio", 0);

    string html = static_cast<string>(r.text);
    parsearHtml(html, url, 0);

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

void Peticion::procesarUrlRecursivo(string url, int nivelActual) {
    if (colaPrioridad.getLongitud() >= limitePaginas) return;
    if (paginasVisitadas.find(url) != paginasVisitadas.end()) return;

    paginasVisitadas.insert(url);

    auto r = cpr::Get(cpr::Url{static_cast<string_view>(url)});
    
    if (r.status_code == 200) {
        parsearHtml(r.text, url, nivelActual);
    }
}

void Peticion::procesarLinks(vector<string> urlsRecolectadas, string urlPadre, int nivelPadre){
    int nuevoNivel = nivelPadre + 1;

    for (const string& url : urlsRecolectadas){
        if (colaPrioridad.getLongitud() >= limitePaginas) return;

        bool insertar = false;

        if (usarProfundidad) {
            if (nuevoNivel <= nivelProfundidad) insertar = true;
        } else {
            insertar = true;
        }

        if (insertar && paginasVisitadas.find(url) == paginasVisitadas.end()) {
            int prioridad = url.length();
            colaPrioridad.insertarUrl(url, prioridad, urlPadre, nuevoNivel);
            
            if (usarProfundidad && nuevoNivel < nivelProfundidad) {
                procesarUrlRecursivo(url, nuevoNivel);
            }
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
        if (usarProfundidad || (colaPrioridad.getLongitud() + urlsRecolectadas.size() < limitePaginas)) { // <--- CAMBIO CLAVE
             string urlEncontrada = static_cast<string>(href->value);
             if (urlEncontrada.find("/") == 0) {
                 string dominioBase = extraerDominio(urlSolicitada);
                 urlEncontrada = "http://" + dominioBase + urlEncontrada;
             }

             if (urlEncontrada.find("http") == 0) {
                 string dominioPadre = extraerDominio(urlSolicitada);
                 string dominioHijo = extraerDominio(urlEncontrada);

                 if (dominioPadre == dominioHijo) {
                     urlsRecolectadas.push_back(urlEncontrada);
                 }
             }
        }
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
    rutaEncontrada = "";

    colaPrioridad.recorrerCola([&](string url, int prioridad, string padre, int nivel) -> bool {
        
        if (url.find(palabraClave) != std::string::npos) {
            std::stringstream ss;
            
            ss << "=== URL ENCONTRADA ===\n";
            ss << "URL: " << url << "\n";
            ss << "Nivel de profundidad: " << nivel << "\n\n";
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
            ss << "--- Ruta de Navegación (Clicks) ---\n";
            ss << "Pasos necesarios: " << camino.size() << "\n\n";
            
            for (int i = camino.size() - 1; i >= 0; i--) {
                ss << "[" << (camino.size() - 1 - i) << "] " << camino[i] << "\n";
                if (i > 0) ss << "  |\n  V\n";
            }
            ss << "===============================\n";
            rutaEncontrada = ss.str();
            
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
        resultado += to_string(contador) + ". " + url + " Nivel: " + to_string(nivel) + "\n";
        contador++;
        return false; // Retornar false para seguir recorriendo
    });
    
    return resultado;
}


bool Peticion::datosCola(){
    return colaPrioridad.colaVacia();
}

string Peticion::obtenerRuta() {
    return rutaEncontrada;
}