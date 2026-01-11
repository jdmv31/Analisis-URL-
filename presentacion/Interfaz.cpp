#include "Interfaz.h"
#include <gtkmm.h>
#include <iostream>

using namespace std;

MiVentana::MiVentana() 
    : m_caja(Gtk::Orientation::VERTICAL, 10),
      m_etiqueta("Web Crawler"),
      m_boton("Presióname") 
{
    // Configuración de la ventana
    set_title("Ejemplo Gtkmm");
    set_default_size(300, 200);

    // Margen interno para la caja
    m_caja.set_margin(20);
    set_child(m_caja);

    // Agregar la etiqueta y el botón a la caja
    m_caja.append(m_etiqueta);
    m_caja.append(m_boton);

    // Conectar la señal del botón
    m_boton.signal_clicked().connect(sigc::mem_fun(*this, &MiVentana::al_hacer_click));
}

void MiVentana::al_hacer_click() {
    m_etiqueta.set_text("¡Gracias por interactuar!");
    std::cout << "Botón presionado" << std::endl;
}