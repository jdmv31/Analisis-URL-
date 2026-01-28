#include "Interfaz.h"
#include <iostream>
#include <negocios/Peticion.h>
#include <string>
#include <algorithm>
#include <cctype>

using std::string;

Peticion peticion;

// =========================================================
// 1. IMPLEMENTACIÓN VISTA RECOPILACIÓN (VALIDADA Y FUSIONADA)
// =========================================================
VistaRecopilacion::VistaRecopilacion(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    // --- VISTA 1: INPUT ---
    m_CardBox.add_css_class("menu-card");
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Recopilación de Links");
    m_LblTitulo.add_css_class("titulo-label");

    m_LblInstruccion.set_markup("<span size='x-large' weight='bold'>Introduce la URL semilla:</span>");
    m_LblInstruccion.add_css_class("texto-instruccion");

    m_EntryUrl.set_placeholder_text("https://sitio-a-recopilar.com");
    m_EntryUrl.add_css_class("entry-url");
    m_EntryUrl.set_size_request(400, -1);

    // Configuración Label Error (Tu código)
    m_LblError.set_markup(""); 
    m_LblError.set_wrap(true);

    // Configuración de Radios (Código de compañero)
    m_RadioProfundidad.set_label("Límite por Profundidad");
    m_RadioPaginas.set_label("Límite por Máx. Páginas");
    m_RadioPaginas.set_group(m_RadioProfundidad);
    m_RadioProfundidad.set_active(true);

    m_RadioBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_RadioBox.set_spacing(5);
    m_RadioBox.append(m_RadioProfundidad);
    m_RadioBox.append(m_RadioPaginas);

    // Input Limite (Código de compañero)
    m_EntryLimite.set_placeholder_text("Límite (ej: 3 niveles o 25 paginas)");
    m_EntryLimite.add_css_class("entry-url");

    m_BtnAnalizar.set_label("Comenzar Recopilación");
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir");

    // FUSION: Armado final preservando tu orden visual pero con los campos de él
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInstruccion);
    m_CardBox.append(m_EntryUrl);
    m_CardBox.append(m_RadioBox);    // Agregado
    m_CardBox.append(m_EntryLimite); // Agregado
    m_CardBox.append(m_BtnAnalizar);
    m_CardBox.append(m_BtnVolver);
    m_CardBox.append(m_LblError);    

    // --- VISTA 2: RESULTADOS ---
    m_CardBoxResultados.add_css_class("menu-card");
    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(15);
    m_CardBoxResultados.set_margin(50);
    m_CardBoxResultados.set_valign(Gtk::Align::CENTER);
    m_CardBoxResultados.set_halign(Gtk::Align::CENTER);

    m_LblResTitulo.set_text("Información Encontrada");
    m_LblResTitulo.add_css_class("titulo-label");

    m_LblResUrl.set_markup("<b>URL:</b> ...");
    
    m_TxtInfoContenido.set_editable(false);
    m_TxtInfoContenido.set_wrap_mode(Gtk::WrapMode::WORD);
    m_TxtInfoContenido.add_css_class("entry-url"); 

    m_ScrollInfo.set_child(m_TxtInfoContenido);
    m_ScrollInfo.set_size_request(450, 200);
    m_ScrollInfo.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    m_BtnGuardar.set_label("Guardar en Cola de Prioridad");
    m_BtnDescartar.set_label("Descartar / Volver");
    m_BtnDescartar.add_css_class("btn-salir");

    m_CardBoxResultados.append(m_LblResTitulo);
    m_CardBoxResultados.append(m_LblResUrl);
    m_CardBoxResultados.append(m_ScrollInfo);
    m_CardBoxResultados.append(m_BtnGuardar);
    m_CardBoxResultados.append(m_BtnDescartar);

    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecopilacion::on_volver_clicked));
    m_BtnAnalizar.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecopilacion::on_analizar_clicked));
    m_BtnGuardar.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecopilacion::on_guardar_clicked));
    m_BtnDescartar.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecopilacion::on_cancelar_resultados_clicked));
}

void VistaRecopilacion::on_volver_clicked() { 
    m_LblError.set_markup(""); 
    m_notebook.set_current_page(0); 
}

void VistaRecopilacion::on_analizar_clicked() {
    string url = m_EntryUrl.get_text();
    string textoLimite = m_EntryLimite.get_text();
    m_LblError.set_markup(""); 

    if (url.empty()) {
        m_LblError.set_markup("<span color='#FF5555' weight='bold'>ADVERTENCIA: Debe ingresar una URL.</span>");
        return;
    }

    int valorLimite = 0;
    try {
        valorLimite = std::stoi(textoLimite);
    } catch (...) {
        valorLimite = 25;
    }

    bool esProfundidad = m_RadioProfundidad.get_active();
    bool huboAjuste = false;

    if (esProfundidad) {
        const int MIN_PROF = 1;
        const int MAX_PROF = 5;

        if (valorLimite < MIN_PROF || valorLimite > MAX_PROF) {
            if (valorLimite < MIN_PROF) valorLimite = MIN_PROF;
            if (valorLimite > MAX_PROF) valorLimite = MAX_PROF;

            m_LblError.set_markup("<span color='#FF5555' weight='bold'>NOTA: Profundidad ajustada a " 
                                  + std::to_string(valorLimite) + " (Rango permitido: 1-5).</span>");
            huboAjuste = true;
        }
    } else {
        const int MIN_PAGS = 25;
        const int MAX_PAGS = 50;

        if (valorLimite < MIN_PAGS || valorLimite > MAX_PAGS) {
            if (valorLimite < MIN_PAGS) valorLimite = MIN_PAGS;
            if (valorLimite > MAX_PAGS) valorLimite = MAX_PAGS;

            m_LblError.set_markup("<span color='#FF5555' weight='bold'>NOTA: Cantidad de páginas ajustada a " 
                                  + std::to_string(valorLimite) + " (Rango permitido: 25-50).</span>");
            huboAjuste = true;
        }
    }
    peticion.configurar(esProfundidad, valorLimite);

    auto procesarYMostrar = [this, url]() {
        m_LblError.set_markup(""); 

        int estado = peticion.realizarPeticion(url);
        if (estado == 200) {
            m_LblResUrl.set_markup("<b>URL Analizada:</b> " + url);
            
            auto buffer = m_TxtInfoContenido.get_buffer();
            buffer->set_text("Análisis completado exitosamente.\nDatos listos para guardar.");
            
            m_CenterBox.set_center_widget(m_CardBoxResultados);
        } else {
            m_LblError.set_markup("<span color='#FF5555' weight='bold'>ERROR DE CONEXIÓN: Código " + std::to_string(estado) + ".\nVerifique la URL o su conexión a internet.</span>");
        }
    };

    if (huboAjuste) {
        Glib::signal_timeout().connect_once(procesarYMostrar, 2000);
    } else {
        procesarYMostrar();
    }
}

void VistaRecopilacion::on_guardar_clicked() {
    peticion.guardarInformacion();
    auto buffer = m_TxtInfoContenido.get_buffer();
    buffer->set_text("Datos almacenados correctamente en memoria.");

    Glib::signal_timeout().connect_once([this]() {        
        m_EntryUrl.set_text("");
        m_CenterBox.set_center_widget(m_CardBox);
        m_notebook.set_current_page(0);

    }, 1500);
}

void VistaRecopilacion::on_cancelar_resultados_clicked() {
    m_CenterBox.set_center_widget(m_CardBox);
}


// =========================================================
// 2. IMPLEMENTACIÓN VISTA BÚSQUEDA (FUSIONADA)
// =========================================================
VistaBusqueda::VistaBusqueda(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    // Vista Input
    m_CardBox.add_css_class("menu-card");
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Buscar Palabra Clave");
    m_LblTitulo.add_css_class("titulo-label");

    m_LblInstruccion.set_markup("<span size='large' weight='bold'>Palabra a buscar (solo letras):</span>");
    m_LblInstruccion.add_css_class("texto-instruccion");
    
    m_EntryKeyword.set_placeholder_text("Ej: tecnologia");
    m_EntryKeyword.add_css_class("entry-url");

    m_LblUrl.set_markup("<span size='large' weight='bold'>URL objetivo:</span>");
    m_LblUrl.add_css_class("texto-instruccion");

    m_EntryUrl.set_placeholder_text("https://sitio.com");
    m_EntryUrl.add_css_class("entry-url");

    // Configuración Labels Error (Tu código)
    m_LblError.set_markup(""); 
    m_LblError.set_wrap(true);

    // Controles de Límite (Codigo de él)
    m_EntryLimite.set_placeholder_text("Límite profundidad: (1 - 5)");
    m_EntryLimite.add_css_class("entry-url");  

    m_RadioBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_RadioBox.set_spacing(5);                                    

    m_BtnBuscar.set_label("Buscar");
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir");

    // FUSION DEL ARMADO
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInstruccion);
    m_CardBox.append(m_EntryKeyword); // Palabra
    m_CardBox.append(m_LblUrl);
    m_CardBox.append(m_EntryUrl);     // URL
    m_CardBox.append(m_EntryLimite);  // Limite 
    m_CardBox.append(m_RadioBox);     // Radios 
    m_CardBox.append(m_BtnBuscar);
    m_CardBox.append(m_BtnVolver);
    m_CardBox.append(m_LblError);   // Error 

    // Vista Resultados
    m_CardBoxResultados.add_css_class("menu-card");
    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(15);
    m_CardBoxResultados.set_margin(50);
    m_CardBoxResultados.set_valign(Gtk::Align::CENTER);
    m_CardBoxResultados.set_halign(Gtk::Align::CENTER);

    m_LblResTitulo.set_text("Resultado de Búsqueda");
    m_LblResTitulo.add_css_class("titulo-label");

    m_LblResDato.set_markup("Resultados para: ...");
    
    m_TxtResContenido.set_editable(false);
    m_TxtResContenido.set_wrap_mode(Gtk::WrapMode::WORD);
    m_TxtResContenido.add_css_class("entry-url");

    m_ScrollRes.set_child(m_TxtResContenido);
    m_ScrollRes.set_size_request(450, 200);
    m_ScrollRes.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    m_BtnNuevaBusqueda.set_label("Nueva Búsqueda");
    m_BtnIrMenu.set_label("Volver al Menú");
    m_BtnIrMenu.add_css_class("btn-salir");

    m_CardBoxResultados.append(m_LblResTitulo);
    m_CardBoxResultados.append(m_LblResDato);
    m_CardBoxResultados.append(m_ScrollRes);
    m_CardBoxResultados.append(m_BtnNuevaBusqueda);
    m_CardBoxResultados.append(m_BtnIrMenu);

    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_volver_clicked));
    m_BtnBuscar.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_buscar_clicked));
    m_BtnNuevaBusqueda.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_nueva_busqueda_clicked));
    m_BtnIrMenu.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_volver_clicked));
}

bool VistaBusqueda::validar_texto(const std::string& texto) {
    if (texto.empty()) return false;
    return std::all_of(texto.begin(), texto.end(), [](unsigned char c){ return std::isalpha(c); });
}

void VistaBusqueda::on_volver_clicked() { 
    m_LblError.set_markup("");
    m_notebook.set_current_page(0); 
}

void VistaBusqueda::on_nueva_busqueda_clicked() {
    m_LblError.set_markup("");
    m_EntryKeyword.set_text(""); 
    m_CenterBox.set_center_widget(m_CardBox);
}

void VistaBusqueda::on_buscar_clicked() {
    string palabra = m_EntryKeyword.get_text();
    string url = m_EntryUrl.get_text();
    string textoLimite = m_EntryLimite.get_text();
    m_LblError.set_markup(""); 

    if (palabra.empty() || url.empty()) {
        m_LblError.set_markup("<span color='#FF5555' weight='bold'>ADVERTENCIA: Todos los campos son obligatorios.</span>");
        return;
    }
    if (!validar_texto(palabra)) {
        m_LblError.set_markup("<span color='#FF5555' weight='bold'>ADVERTENCIA: La palabra clave solo debe contener letras.</span>");
        return;
    }

    int valorLimite = 0;
    try {
        if (!textoLimite.empty()) valorLimite = std::stoi(textoLimite);
    } catch (...) {
        valorLimite = 1;
    }

    const int MIN_PROF_SEGURIDAD = 1;
    const int MAX_PROF_SEGURIDAD = 5;
    

    if (valorLimite < MIN_PROF_SEGURIDAD || valorLimite > MAX_PROF_SEGURIDAD) {

        if (valorLimite < MIN_PROF_SEGURIDAD) {
            valorLimite = MIN_PROF_SEGURIDAD;
        } else {
            valorLimite = MAX_PROF_SEGURIDAD;
        }

        m_LblError.set_markup("<span color='#e6a00f' weight='bold'>NOTA: La profundidad debe estar entre " 
                              + std::to_string(MIN_PROF_SEGURIDAD) + " y " 
                              + std::to_string(MAX_PROF_SEGURIDAD) + ". Se ajustó a " 
                              + std::to_string(valorLimite) + ".</span>");
    }

    peticion.configurar(true, valorLimite); 

    int estado = peticion.realizarPeticion(url);
    if (estado != 200) {
        m_LblError.set_markup("<span color='#FF5555' weight='bold'>ERROR: No se pudo acceder a la URL (Código " + std::to_string(estado) + ").</span>");
        return;
    }

    bool encontrada = peticion.buscarPalabra(palabra);

    m_LblResDato.set_markup("<b>Búsqueda:</b> " + palabra + "\n<b>En:</b> " + url);
    auto buffer = m_TxtResContenido.get_buffer();
    
    if (encontrada) {
         buffer->set_text(peticion.obtenerRuta());
    } else {
         buffer->set_text("La palabra no se encontró en los niveles analizados (Profundidad: " + std::to_string(valorLimite) + ").");
    }
    m_CenterBox.set_center_widget(m_CardBoxResultados);
}


// =========================================================
// 3. IMPLEMENTACIÓN VISTA ANÁLISIS (MODIFICADA)
// =========================================================
VistaAnalisis::VistaAnalisis(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    // ================== VISTA 1: INPUT ==================
    m_CardBox.add_css_class("menu-card");
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Análisis Estructural");
    m_LblTitulo.add_css_class("titulo-label");

    m_LblInfo.set_markup("<span size='x-large' weight='bold'>Visualizar métricas</span>");
    m_LblInfo.add_css_class("texto-instruccion");

    m_LblError.set_markup(""); 
    m_LblError.set_wrap(true);

    m_BtnAnalizar.set_label("Ejecutar Análisis");
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir");

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInfo); // Añadido
    m_CardBox.append(m_BtnAnalizar);
    m_CardBox.append(m_BtnVolver);
    m_CardBox.append(m_LblError);
    
    // ================== VISTA 2: RESULTADOS (REPORTE) ==================
    m_CardBoxResultados.add_css_class("menu-card");
    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(10);
    m_CardBoxResultados.set_margin(50);
    m_CardBoxResultados.set_valign(Gtk::Align::CENTER);
    m_CardBoxResultados.set_halign(Gtk::Align::CENTER);

    m_LblResTitulo.set_text("Reporte de Métricas");
    m_LblResTitulo.add_css_class("titulo-label");

    // Estilo tipo 'caja blanca' para resaltar los datos
    m_LblHuerfanas.add_css_class("entry-url");
    m_LblImagenes.add_css_class("entry-url");
    m_LblPromedio.add_css_class("entry-url");
    m_LblVisitadas.add_css_class("entry-url");

    m_BtnCerrarResultado.set_label("Volver / Nuevo Análisis");
    m_BtnCerrarResultado.add_css_class("btn-salir");

    m_CardBoxResultados.append(m_LblResTitulo);
    m_CardBoxResultados.append(m_LblHuerfanas);
    m_CardBoxResultados.append(m_LblImagenes);
    m_CardBoxResultados.append(m_LblPromedio);
    m_CardBoxResultados.append(m_LblVisitadas);
    m_CardBoxResultados.append(m_BtnCerrarResultado);

    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    // Conexiones
    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaAnalisis::on_volver_clicked));
    m_BtnAnalizar.signal_clicked().connect(sigc::mem_fun(*this, &VistaAnalisis::on_analizar_clicked));
    m_BtnCerrarResultado.signal_clicked().connect(sigc::mem_fun(*this, &VistaAnalisis::on_cerrar_resultados_clicked));
}

void VistaAnalisis::on_analizar_clicked() { 
    m_LblError.set_markup(""); 

    // Nota: Asumimos que datosCola() devuelve true si la cola ESTÁ VACÍA (basado en tu código de VistaEnlaces)
    if (peticion.datosCola()) {
        m_LblError.set_markup("<span color='#FF5555' weight='bold'>COLA VACIA SIN METRICAS QUE ENSENAR</span>");
        return;
    }

    // Si hay datos, calculamos y mostramos
    std::cout << "Calculando métricas de datos en memoria..." << std::endl; 
    
    peticion.calcularMetricas();

    // Extraemos los datos usando los getters de Peticion.h
    m_LblHuerfanas.set_markup("<b>Páginas Huérfanas:</b> " + std::to_string(peticion.getPaginasHuerfanas()));
    m_LblImagenes.set_markup("<b>Cantidad de Imágenes:</b> " + std::to_string(peticion.getCantIMG()));
    m_LblPromedio.set_markup("<b>Promedio de Enlaces:</b> " + std::to_string(peticion.getPromedioLinks()));
    m_LblVisitadas.set_markup("<b>Páginas Visitadas:</b> " + std::to_string(peticion.getCantNodos()));

    // Cambiamos a la vista de reporte
    m_CenterBox.set_center_widget(m_CardBoxResultados);
}

// Asegúrate de modificar también el on_cerrar para no llamar a m_EntryUrl
void VistaAnalisis::on_cerrar_resultados_clicked() {
    m_CenterBox.set_center_widget(m_CardBox);
}

void VistaAnalisis::on_volver_clicked() { 
    m_LblError.set_markup(""); 
    m_notebook.set_current_page(0); 
}

// =========================================================
// 4. IMPLEMENTACIÓN VISTA ENLACES (MANTENIDA TU VERSION)
// =========================================================
VistaEnlaces::VistaEnlaces(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    // --- VISTA 1: MENÚ INICIAL ---
    m_CardBox.add_css_class("menu-card");
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(5);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Enlaces Almacenados");
    m_LblTitulo.add_css_class("titulo-label");

    m_LblInfo.set_markup("<span size='x-large' weight='bold'>Visualizar base de datos</span>");
    m_LblInfo.add_css_class("texto-instruccion");

    m_LblError.set_markup(""); 
    m_LblError.set_wrap(true);

    m_BtnMostrar.set_label("Cargar Lista");
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir");

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInfo);
    m_CardBox.append(m_BtnMostrar);
    m_CardBox.append(m_BtnVolver); 
    m_CardBox.append(m_LblError); 

    // --- VISTA 2: RESULTADOS (LISTA) ---
    m_CardBoxResultados.add_css_class("menu-card");
    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(15);
    m_CardBoxResultados.set_margin(50);
    m_CardBoxResultados.set_valign(Gtk::Align::CENTER);
    m_CardBoxResultados.set_halign(Gtk::Align::CENTER);

    m_LblResTitulo.set_text("Cola de Prioridad");
    m_LblResTitulo.add_css_class("titulo-label");

    // Configurar área de texto (Read-Only)
    m_TxtResContenido.set_editable(false);
    m_TxtResContenido.set_monospace(true);
    m_TxtResContenido.set_wrap_mode(Gtk::WrapMode::WORD);
    m_TxtResContenido.add_css_class("entry-url"); 

    // Configurar Scroll
    m_ScrollRes.set_child(m_TxtResContenido);
    m_ScrollRes.set_size_request(550, 300);
    m_ScrollRes.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    m_BtnCerrarLista.set_label("Cerrar / Volver");
    m_BtnCerrarLista.add_css_class("btn-salir");

    m_CardBoxResultados.append(m_LblResTitulo);
    m_CardBoxResultados.append(m_ScrollRes);
    m_CardBoxResultados.append(m_BtnCerrarLista);

    // Inicialización
    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    // Conexiones de Señales
    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaEnlaces::on_volver_clicked));
    m_BtnMostrar.signal_clicked().connect(sigc::mem_fun(*this, &VistaEnlaces::on_mostrar_clicked));
    m_BtnCerrarLista.signal_clicked().connect(sigc::mem_fun(*this, &VistaEnlaces::on_cerrar_lista_clicked));
}

void VistaEnlaces::on_volver_clicked() { 
    m_LblError.set_markup(""); 
    m_notebook.set_current_page(0); 
}

void VistaEnlaces::on_mostrar_clicked() { 
    m_LblError.set_markup(""); 

    // Validamos antes de cambiar de vista
    if (peticion.datosCola()) {
        m_LblError.set_markup("<span color='#FF5555' weight='bold'>LA COLA ESTÁ VACÍA. NO HAY ENLACES.</span>");
        return; // SE QUEDA EN LA VISTA 1
    } 
    
    // Si hay datos, procede normal
    std::string listado = peticion.obtenerListado(); 
    auto buffer = m_TxtResContenido.get_buffer();
    buffer->set_text(listado);
    
    std::cout << "Mostrando vista de enlaces..." << std::endl;
    m_CenterBox.set_center_widget(m_CardBoxResultados); // CAMBIO A VISTA 2
}

void VistaEnlaces::on_cerrar_lista_clicked() {
    m_CenterBox.set_center_widget(m_CardBox); // Regreso al menú interno
}


// =========================================================
// IMPLEMENTACIÓN INTERFAZ PRINCIPAL
// =========================================================
Interfaz::Interfaz() {
    set_title("Web Crawler");
    set_default_size(800, 550);

    const std::string ESTILO_CSS = R"(
        window, notebook, stack { 
            background-color: #222223; 
        }

        .menu-card { 
            background-color: #BCB4FF; 
            border-radius: 15px; 
            /* REDUCIDO: De 40px a 25px */
            padding: 25px; 
            min-width: 500px; 
            /* REDUCIDO: Altura mínima menor */
            min-height: 300px;
            box-shadow: 0px 4px 15px rgba(0,0,0,0.5); 
        }

        .titulo-label {
            color: #222223;
            /* REDUCIDO: De 45px a 35px */
            font-size: 35px;
            font-weight: bold;
            /* REDUCIDO: De 25px a 15px */
            margin-bottom: 15px;
        }

        .menu-card button { 
            /* REDUCIDO: Margen de 10px a 5px para juntarlos más */
            margin: 5px; 
            /* REDUCIDO: Padding de 18px a 10px (hace el botón más flaco) */
            padding: 14px; 
            min-width: 260px; 
            border-radius: 10px; 
            font-weight: 600;
            /* REDUCIDO: Letra de 23px a 18px */
            font-size: 21px;
        }

        .menu-card button:hover { 
            background-color: #e30909; 
            color: #222223; 
        }

        .btn-salir { 
            background-color: #A9F527; 
            background-image: none;
            color: #222223; 
            border: 1px solid #8bc34a; 
            font-weight: bold;
        }

        .btn-salir:hover { 
            background-color: #92d422; 
            color: #000000;
        }
            
        header {
            background-color: #333333; 
        }
        
        tab {
            background-color: #444444;
            color: white;
            padding: 5px 10px;
            margin: 2px;
        }
        
        tab:checked {
            background-color: #92d422; 
            color: #222223;
            font-weight: bold;
        }

        .texto-instruccion {
            font-size: 20px;
            font-weight: bold;
            color: #222223;
            margin-bottom: 15px;
        }

        .entry-url {
            font-size: 20px;
            padding: 15px;
            border-radius: 5px;
            color: black;
            background-color: white;
            border: 2px solid #8bc34a;  
        }
    )";

    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_data(ESTILO_CSS);
    Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    m_Notebook.set_scrollable(true);
    m_Notebook.set_show_tabs(true);
    m_Notebook.set_show_border(false);

    // --- PESTAÑA 0: INICIO ---
    m_titulo_principal.set_text("MENÚ PRINCIPAL");
    m_titulo_principal.add_css_class("titulo-label");

    m_MenuContainer.add_css_class("menu-card");
    m_MenuContainer.set_orientation(Gtk::Orientation::VERTICAL);
    m_MenuContainer.set_spacing(10);
    m_MenuContainer.set_margin(30);
    m_MenuContainer.set_valign(Gtk::Align::CENTER);
    m_MenuContainer.set_halign(Gtk::Align::CENTER);

    m_Btn1.set_label("Recopilar links");
    m_Btn2.set_label("Buscar palabra clave");
    m_Btn3.set_label("Realizar análisis estructural");
    m_Btn4.set_label("Mostrar enlaces almacenados");
    m_BtnSalir.set_label("Cerrar Aplicación");
    m_BtnSalir.add_css_class("btn-salir");

    m_MenuContainer.append(m_titulo_principal);
    m_MenuContainer.append(m_Btn1);
    m_MenuContainer.append(m_Btn2);
    m_MenuContainer.append(m_Btn3);
    m_MenuContainer.append(m_Btn4);
    m_MenuContainer.append(m_BtnSalir);

    m_CenterBoxInicio.set_center_widget(m_MenuContainer);
    m_Notebook.append_page(m_CenterBoxInicio, "Inicio");

    auto* vistaRecopilar = Gtk::make_managed<VistaRecopilacion>(m_Notebook);
    m_Notebook.append_page(*vistaRecopilar, "Recopilar");

    auto* vistaBuscar = Gtk::make_managed<VistaBusqueda>(m_Notebook);
    m_Notebook.append_page(*vistaBuscar, "Buscar");

    auto* vistaAnalisis = Gtk::make_managed<VistaAnalisis>(m_Notebook);
    m_Notebook.append_page(*vistaAnalisis, "Análisis");

    auto* vistaEnlaces = Gtk::make_managed<VistaEnlaces>(m_Notebook);
    m_Notebook.append_page(*vistaEnlaces, "Enlaces");

    m_Btn1.signal_clicked().connect([this]{ m_Notebook.set_current_page(1); });
    m_Btn2.signal_clicked().connect([this]{ m_Notebook.set_current_page(2); });
    m_Btn3.signal_clicked().connect([this]{ m_Notebook.set_current_page(3); });
    m_Btn4.signal_clicked().connect([this]{ m_Notebook.set_current_page(4); });
    m_BtnSalir.signal_clicked().connect(sigc::mem_fun(*this, &Interfaz::on_salir_clicked));

    set_child(m_Notebook);
    peticion.leerInformacion();

    /*
    if (!peticion.datosCola())
        peticion.calcularMetricas();
    */    
}

void Interfaz::on_salir_clicked() { 
    hide();
}