#pragma once
#include <gtkmm.h>
#include <string>
#include <negocios/Peticion.h>

class Interfaz : public Gtk::Window {
    public:
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
        Peticion peticion;

    public:
        Interfaz();
        virtual ~Interfaz();

    protected:
        void on_boton_nueva_ventana_click();
        void on_boton_salir_click();
        void cargar_css();
        void errorOcurrido(std::string mensaje);
};

