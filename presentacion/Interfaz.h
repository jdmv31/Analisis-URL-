#pragma once
#include <gtkmm.h>


class MiVentana : public Gtk::Window {
public:
    MiVentana();
    virtual ~MiVentana();

protected:
    // Señales
    void on_boton_nueva_ventana_click();
    void on_boton_salir_click(); // <--- NUEVA FUNCIÓN
    void cargar_css();

    // Widgets Principales
    Gtk::ScrolledWindow m_scrolled_window; // <--- NUEVO CONTENEDOR
    Gtk::Box m_caja_principal;
    
    // Títulos y Textos
    Gtk::Label m_titulo_principal;
    Gtk::Label m_lbl_mensaje;
    Gtk::Label m_lbl_url;
    
    // Entradas
    Gtk::Entry m_entry_url;

    // Radio Botones
    Gtk::Box m_caja_radios;
    Gtk::CheckButton m_radio1;
    Gtk::CheckButton m_radio2;

    // Botones
    Gtk::Button m_boton_accion;
    Gtk::Button m_boton_salir; // <--- NUEVO BOTÓN

    // Ventana Secundaria (Modal)
    Gtk::Window m_segunda_ventana;
    Gtk::Box m_caja_segunda;
    Gtk::Label m_lbl_segunda_ventana;
};

