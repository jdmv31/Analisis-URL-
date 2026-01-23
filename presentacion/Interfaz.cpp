#include "Interfaz.h"
#include <iostream>
#include <negocios/Peticion.h>
#include <string>

using std::string;

Peticion peticion;

// =========================================================
// 1. IMPLEMENTACIÓN VISTA RECOPILACIÓN (COMPLETA)
// =========================================================
VistaRecopilacion::VistaRecopilacion(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    // Configuración Base de la Tarjeta
    m_CardBox.add_css_class("menu-card");
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    // Título y Entrada
    m_LblTitulo.set_text("Recopilación de Links");
    m_LblTitulo.add_css_class("titulo-label");

    m_LblInstruccion.set_markup("<span size='x-large' weight='bold'>Introduce la URL semilla:</span>");
    m_LblInstruccion.add_css_class("texto-instruccion");

    m_EntryUrl.set_placeholder_text("https://sitio-a-recopilar.com");
    m_EntryUrl.add_css_class("entry-url");
    m_EntryUrl.set_size_request(400, -1);

    // --- RADIOS (Solo aquí) ---
    m_RadioProfundidad.set_label("Límite por Profundidad");
    m_RadioPaginas.set_label("Límite por Máx. Páginas");
    m_RadioPaginas.set_group(m_RadioProfundidad); // Agrupar
    m_RadioProfundidad.set_active(true);

    m_RadioBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_RadioBox.set_spacing(5);
    m_RadioBox.append(m_RadioProfundidad);
    m_RadioBox.append(m_RadioPaginas);

    // Botones
    m_BtnAnalizar.set_label("Comenzar Recopilación");
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir");

    // Armado
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInstruccion);
    m_CardBox.append(m_EntryUrl);
    m_CardBox.append(m_RadioBox); // Agregamos radios
    m_CardBox.append(m_BtnAnalizar);
    m_CardBox.append(m_BtnVolver);

    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    // Señales
    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecopilacion::on_volver_clicked));
    m_BtnAnalizar.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecopilacion::on_analizar_clicked));
}

void VistaRecopilacion::on_volver_clicked() { m_notebook.set_current_page(0); }
void VistaRecopilacion::on_analizar_clicked() {
    string url = m_EntryUrl.get_text();
    if (!url.empty()){
        int estado = peticion.realizarPeticion(url);
        if (estado == 200)
            peticion.guardarInformacion();
        // agregale algo para los mensajes nicole
    }
}


// =========================================================
// 2. IMPLEMENTACIÓN VISTA BÚSQUEDA (SIN RADIOS)
// =========================================================
VistaBusqueda::VistaBusqueda(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    m_CardBox.add_css_class("menu-card");
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Buscar Palabra Clave");
    m_LblTitulo.add_css_class("titulo-label");

    // Texto diferente
    m_LblInstruccion.set_markup("<span size='x-large' weight='bold'>Palabra a buscar:</span>");
    m_LblInstruccion.add_css_class("texto-instruccion");

    m_EntryKeyword.set_placeholder_text("Ej: Tecnología, Noticias...");
    m_EntryKeyword.add_css_class("entry-url");
    m_EntryKeyword.set_size_request(400, -1);

    m_EntryUrl.set_placeholder_text("https://sitio-a-recopilar.com");
    m_EntryUrl.add_css_class("entry-url");
    m_EntryUrl.set_size_request(400, -1);

    m_BtnBuscar.set_label("Buscar");
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir");

    // Armado (SIN RADIOS)
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInstruccion);
    m_CardBox.append(m_EntryKeyword);
    m_CardBox.append(m_EntryUrl);
    m_CardBox.append(m_BtnBuscar);
    m_CardBox.append(m_BtnVolver);
   

    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_volver_clicked));
    m_BtnBuscar.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_buscar_clicked));
}

void VistaBusqueda::on_volver_clicked() { m_notebook.set_current_page(0); }
void VistaBusqueda::on_buscar_clicked() {
    string palabra = m_EntryKeyword.get_text();
    string url = m_EntryUrl.get_text();
    bool encontrado = false;
    if (!url.empty()){
        int estado = peticion.realizarPeticion(url);
        if (estado == 200)
            peticion.buscarPalabra(palabra);
    }

    /*
        if (!encontrado)
    */

}


// =========================================================
// 3. IMPLEMENTACIÓN VISTA ANÁLISIS (SIN RADIOS)
// =========================================================
VistaAnalisis::VistaAnalisis(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    m_CardBox.add_css_class("menu-card");
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Análisis Estructural");
    m_LblTitulo.add_css_class("titulo-label");

    m_LblInstruccion.set_markup("<span size='x-large' weight='bold'>URL para analizar:</span>");
    m_LblInstruccion.add_css_class("texto-instruccion");

    m_EntryUrl.set_placeholder_text("https://sitio.com");
    m_EntryUrl.add_css_class("entry-url");
    m_EntryUrl.set_size_request(400, -1);

    m_BtnAnalizar.set_label("Ejecutar Análisis");
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir");

    // Armado (SIN RADIOS)
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInstruccion);
    m_CardBox.append(m_EntryUrl);
    m_CardBox.append(m_BtnAnalizar);
    m_CardBox.append(m_BtnVolver);

    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaAnalisis::on_volver_clicked));
    m_BtnAnalizar.signal_clicked().connect(sigc::mem_fun(*this, &VistaAnalisis::on_analizar_clicked));
}

void VistaAnalisis::on_volver_clicked() { m_notebook.set_current_page(0); }
void VistaAnalisis::on_analizar_clicked() { std::cout << "Analizando Estructura..." << std::endl; }


// =========================================================
// 4. IMPLEMENTACIÓN VISTA ENLACES (Solo Botones)
// =========================================================
VistaEnlaces::VistaEnlaces(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    m_CardBox.add_css_class("menu-card");
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Enlaces Almacenados");
    m_LblTitulo.add_css_class("titulo-label");

    m_LblInfo.set_markup("<span size='x-large' weight='bold'>Ver la base de datos</span>");
    m_LblInfo.add_css_class("texto-instruccion");

    m_BtnMostrar.set_label("Cargar Lista");
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir");

    // Armado (Ni inputs ni radios)
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInfo);
    m_CardBox.append(m_BtnMostrar);
    m_CardBox.append(m_BtnVolver);

    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaEnlaces::on_volver_clicked));
    m_BtnMostrar.signal_clicked().connect(sigc::mem_fun(*this, &VistaEnlaces::on_mostrar_clicked));
}

void VistaEnlaces::on_volver_clicked() { m_notebook.set_current_page(0); }
void VistaEnlaces::on_mostrar_clicked() { std::cout << "Mostrando enlaces..." << std::endl; }


// =========================================================
// IMPLEMENTACIÓN INTERFAZ PRINCIPAL
// =========================================================
Interfaz::Interfaz() {
    set_title("Web Crawler");
    set_default_size(900, 900);

    const std::string ESTILO_CSS = R"(
        window, notebook, stack { 
            background-color: #222223; 
        }

        .menu-card { 
            background-color: #BCB4FF; 
            border-radius: 15px; 
            padding: 80px; 
            min-width: 500px;  /* Ancho mínimo */
            min-height: 450px;
            box-shadow: 0px 4px 15px rgba(0,0,0,0.5); 
        }

        .titulo-label {
            color: #222223;
            font-size: 45px;
            font-weight: bold;
            margin-bottom: 25px;
        }

        .menu-card button { 
            margin: 10px; 
            padding: 18px; 
            min-width: 260px; 
            border-radius: 10px; 
            font-weight: 600;
            font-size: 23px;
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
            font-size: 20px;   /* Tamaño de letra */
            font-weight: bold;
            color: #222223;
            margin-bottom: 15px; /* Separación con el input */
        }

        /* Clase para el campo de texto (Entry) */
        .entry-url {
            font-size: 20px;    /* Tamaño del texto que escribes */
            padding: 15px;      /* Hace la caja más "gorda" (relleno interno) */
            border-radius: px; /* Bordes redondeados */
            color: black;
            background-color: white;
            border: 2px solid #8bc34a; /* Un borde verde para que resalte */    
        }
    )";

    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_data(ESTILO_CSS);
    Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Config Notebook
    m_Notebook.set_scrollable(true);
    m_Notebook.set_show_tabs(true);
    m_Notebook.set_show_border(false);

    // --- PESTAÑA 0: INICIO ---
    m_titulo_principal.set_text("MENÚ PRINCIPAL");
    m_titulo_principal.add_css_class("titulo-label");

    m_MenuContainer.add_css_class("menu-card");
    m_MenuContainer.set_orientation(Gtk::Orientation::VERTICAL);
    m_MenuContainer.set_spacing(10);
    m_MenuContainer.set_margin(50); // Margen para que no pegue
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

    // --- AQUÍ INSTANCIAMOS LAS CLASES INDIVIDUALES ---
    
    // 1. Recopilación (Con Radios)
    auto* vistaRecopilar = Gtk::make_managed<VistaRecopilacion>(m_Notebook);
    m_Notebook.append_page(*vistaRecopilar, "Recopilar");

    // 2. Búsqueda (Sin Radios)
    auto* vistaBuscar = Gtk::make_managed<VistaBusqueda>(m_Notebook);
    m_Notebook.append_page(*vistaBuscar, "Buscar");

    // 3. Análisis (Sin Radios)
    auto* vistaAnalisis = Gtk::make_managed<VistaAnalisis>(m_Notebook);
    m_Notebook.append_page(*vistaAnalisis, "Análisis");

    // 4. Enlaces (Solo lista)
    auto* vistaEnlaces = Gtk::make_managed<VistaEnlaces>(m_Notebook);
    m_Notebook.append_page(*vistaEnlaces, "Enlaces");

    // Señales para navegar
    m_Btn1.signal_clicked().connect([this]{ m_Notebook.set_current_page(1); });
    m_Btn2.signal_clicked().connect([this]{ m_Notebook.set_current_page(2); });
    m_Btn3.signal_clicked().connect([this]{ m_Notebook.set_current_page(3); });
    m_Btn4.signal_clicked().connect([this]{ m_Notebook.set_current_page(4); });
    m_BtnSalir.signal_clicked().connect(sigc::mem_fun(*this, &Interfaz::on_salir_clicked));

    set_child(m_Notebook);
    peticion.leerInformacion();
    if (!peticion.datosCola())
        peticion.calcularMetricas();
}

void Interfaz::on_salir_clicked() { 
    hide();
 }

