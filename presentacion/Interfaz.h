

#pragma once
#include <gtkmm.h>
#include <string>
#include <negocios/Peticion.h>


class VentanaSecundaria : public Gtk::Box {
public:
    VentanaSecundaria(const std::string& titulo, Gtk::Notebook& notebook);
    Peticion peticion;

protected:
    void on_volver_clicked();
    void on_analizar_clicked(); // Slot para el botón analizar
    void errorOcurrido(const std::string& mensaje); // Declarar aquí

    // Referencia al notebook
    Gtk::Notebook& m_notebook;

    // Contenedores de diseño
    Gtk::CenterBox m_CenterBox; // Para centrar la tarjeta
    Gtk::Box m_CardBox;         // La tarjeta violeta (contenedor del formulario)
    Gtk::Box m_RadioBox;        // Contenedor horizontal para los radios

    // Widgets del formulario
    Gtk::Label m_LblTitulo;
    
    Gtk::Label m_LblInstruccion;
    Gtk::Entry m_EntryUrl;
    
    Gtk::CheckButton m_RadioProfundidad;
    Gtk::CheckButton m_RadioPaginas;
    
    Gtk::Button m_BtnAnalizar;
    Gtk::Button m_BtnVolver;
};


class Interfaz : public Gtk::Window {
public:
    Interfaz();

protected:
    void on_salir_clicked();

    // Gestor de pestañas
    Gtk::Notebook m_Notebook;

    // --- Pestaña Inicio ---
    Gtk::CenterBox m_CenterBoxInicio;
    Gtk::Box m_MenuContainer;
    Gtk::Label m_titulo_principal;
    Gtk::Button m_Btn1;
    Gtk::Button m_Btn2;
    Gtk::Button m_Btn3;
    Gtk::Button m_Btn4;
    Gtk::Button m_BtnSalir;
};

