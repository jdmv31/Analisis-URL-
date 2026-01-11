#pragma once
#include <gtkmm.h>

class MiVentana : public Gtk::Window {
public:
    MiVentana();

protected:
    // Manejador de señal para el botón
    void al_hacer_click();

    // Componentes de la interfaz
    Gtk::Box m_caja;
    Gtk::Label m_etiqueta;
    Gtk::Button m_boton;
};