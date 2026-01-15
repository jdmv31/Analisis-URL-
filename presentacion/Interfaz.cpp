#include "Interfaz.h"
#include <iostream>

const std::string ESTILO_CSS = R"(
    window {
        background-color: #052E70;
        font-family: 'Segoe UI', sans-serif;
    }
    
    .titulo {
        font-size: 40px;
        font-weight: bold;
        color: #e7e9ec;
        margin-bottom: 20px;
    }

    entry {
        background-color: white;
        color: #8b8484;
        border: 1px solid #ced4da;
        border-radius: 8px;
        padding: 15px;
        box-shadow: none;
        min-height: 35px;
    }

    entry:focus {
        border-color: #3498db;
    }

    /* Estilo Botón Analizar (Azul) */
    button.btn-accion {
        background-image: none;
        background-color: #3498db;
        color: white;
        border-radius: 8px;
        padding: 10px 20px;
        font-weight: bold;
        transition: all 0.2s;
        margin-bottom: 10px;
    }

    button.btn-accion:hover {
        background-color: #2980b9;
    }

    button.btn-accion:active {
        background-color: #1c5980;
    }

    /* Estilo Botón Salir (Rojo) */
    button.btn-salir {
        background-image: none;
        background-color: #e74c3c;
        color: white;
        border-radius: 8px;
        padding: 10px 20px;
        font-weight: bold;
        transition: all 0.2s;
    }

    button.btn-salir:hover {
        background-color: #c0392b;
    }

    button.btn-salir:active {
        background-color: #922b21;
    }

    checkbutton {
        color: #ffffff;
        font-weight: 500;
    }
)";

Interfaz::Interfaz()
    : m_caja_principal(Gtk::Orientation::VERTICAL, 15),
      m_caja_radios(Gtk::Orientation::HORIZONTAL, 20),
      m_radio1("Profundidad Máxima"),
      m_radio2("Número de páginas máximas"),
      m_boton_accion("Analizar sitio"),
      m_boton_salir("Cerrar todo") // Inicializar texto del nuevo botón
{
    // 1. Configuración de la Ventana
    set_title("Web Crawler");
    set_default_size(800, 800);

    // Cargar estilos
    cargar_css();

    // 2. Configurar el Contenedor Scroll (Nuevo)
    // Esto permite que si la ventana es pequeña, aparezca barra de desplazamiento
    m_scrolled_window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_scrolled_window.set_child(m_caja_principal); // La caja principal va DENTRO del scroll
    set_child(m_scrolled_window); // El scroll va DENTRO de la ventana

    // 3. Configurar Widgets
    
    // Título
    m_titulo_principal.set_text("Panel de Control");
    m_titulo_principal.set_halign(Gtk::Align::CENTER);
    m_titulo_principal.add_css_class("titulo");

    // Textos
    m_lbl_mensaje.set_text("Introduce una URL para comenzar");
    m_lbl_url.set_text("URL Objetivo:");
    m_lbl_url.set_halign(Gtk::Align::START);
    m_entry_url.set_placeholder_text("https://ejemplo.com");

    // Radio Botones
    m_radio2.set_group(m_radio1);
    m_radio1.set_active(true);

    // Botones
    m_boton_accion.add_css_class("btn-accion");
    m_boton_accion.set_cursor(Gdk::Cursor::create("pointer"));

    m_boton_salir.add_css_class("btn-salir"); // Clase CSS roja
    m_boton_salir.set_cursor(Gdk::Cursor::create("pointer"));

    // 4. Estructura (Layout) -> Todo va a m_caja_principal
    m_caja_principal.set_margin(30);
    // Nota: No hacemos set_child(m_caja_principal) aquí porque ya lo hicimos en el paso 2 al scroll

    m_caja_principal.append(m_titulo_principal);
    m_caja_principal.append(m_lbl_mensaje);
    m_caja_principal.append(m_lbl_url);
    m_caja_principal.append(m_entry_url);

    // Caja horizontal radios
    m_caja_radios.set_halign(Gtk::Align::CENTER);
    m_caja_radios.append(m_radio1);
    m_caja_radios.append(m_radio2);
    m_caja_principal.append(m_caja_radios);

    // Separador
    auto espacio = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    espacio->set_size_request(-1, 20);
    m_caja_principal.append(*espacio);

    // Botones de acción
    m_caja_principal.append(m_boton_accion);
    
    // Separador pequeño entre botones
    auto espacio_btn = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    espacio_btn->set_size_request(-1, 10);
    m_caja_principal.append(*espacio_btn);

    m_caja_principal.append(m_boton_salir);

    // 5. Conectar Señales
    m_boton_accion.signal_clicked().connect(
        sigc::mem_fun(*this, &Interfaz::on_boton_nueva_ventana_click)
    );

    // Conectar botón de salir
    m_boton_salir.signal_clicked().connect(
        sigc::mem_fun(*this, &Interfaz::on_boton_salir_click)
    );

    // Configuración ventana secundaria
    m_segunda_ventana.set_title("Resultados");
    m_segunda_ventana.set_default_size(800, 800);
    m_segunda_ventana.set_transient_for(*this);
    m_segunda_ventana.set_modal(true); // Bloquear ventana principal mientras esta está abierta
    
    m_caja_segunda.set_orientation(Gtk::Orientation::VERTICAL);
    m_caja_segunda.set_margin(20);
    m_caja_segunda.set_valign(Gtk::Align::CENTER);
    m_lbl_segunda_ventana.set_text("¡Has abierto la nueva ventana!");
    m_lbl_segunda_ventana.add_css_class("titulo");
    
    m_caja_segunda.append(m_lbl_segunda_ventana);
    m_segunda_ventana.set_child(m_caja_segunda);
}

Interfaz::~Interfaz() {
    // asd
}

void Interfaz::cargar_css() {
    auto css_provider = Gtk::CssProvider::create();
    try {
        css_provider->load_from_data(ESTILO_CSS);
        Gtk::StyleContext::add_provider_for_display(
            Gdk::Display::get_default(),
            css_provider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
    } catch (const Gtk::CssParserError& ex) {
        std::cerr << "Error al cargar CSS: " << ex.what() << std::endl;
    }
}

void Interfaz::errorOcurrido(std::string mensaje){
    auto ventanaEmergente = Gtk::AlertDialog::create(static_cast<Glib::ustring>(mensaje));
    ventanaEmergente->set_detail("Error en la ejecucion del programa.");
    ventanaEmergente->show(*this);
}

void Interfaz::on_boton_nueva_ventana_click() {
    std::string url = m_entry_url.get_text();
    // esto es lo que enlaza la interfaz con la logica nicole :)
    if(url.empty()){
        errorOcurrido("No se ingreso ninguna URL, por favor intente nuevamente.");
        return;
    }

    if (peticion.realizarPeticion(url) == 0){
        errorOcurrido("El dominio ingresado es inexistente, por favor intente nuevamente.");
        return;
    }

    if (peticion.realizarPeticion(url) == 400){
        errorOcurrido("Error HTTP en el acceso al dominio, por favor ingrese otra URL.");
        return;
    }

    if (peticion.realizarPeticion(url) == 200){
        peticion.guardarInformacion();
        m_segunda_ventana.set_visible(true);
    }

}

void Interfaz::on_boton_salir_click() {
    m_segunda_ventana.set_visible(false); 
    close(); 
}