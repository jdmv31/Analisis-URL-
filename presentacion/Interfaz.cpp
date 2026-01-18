#include "Interfaz.h"
#include <iostream>

// =========================================================
// IMPLEMENTACIÓN VISTA SECUNDARIA
// =========================================================

VentanaSecundaria::VentanaSecundaria(const std::string& titulo, Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), 
      m_notebook(notebook) 
{
    // 1. Configuración de la tarjeta contenedora (Estilo menu-card)
    m_CardBox.add_css_class("menu-card"); // Usamos el mismo estilo violeta
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(50);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    // 2. Título de la sección
    m_LblTitulo.set_text(titulo);
    m_LblTitulo.add_css_class("titulo-label");
    
    // 3. Campo de URL
    m_LblInstruccion.set_text("Introduce la URL objetivo:");
    // Hacemos que el texto se vea bien (negrita)
    m_LblInstruccion.add_css_class("texto-instruccion");
    m_LblInstruccion.set_markup("<b>Introduce la URL objetivo:</b>");
    
    m_EntryUrl.set_placeholder_text("https://ejemplo.com");
    m_EntryUrl.set_margin_bottom(10);
    m_EntryUrl.add_css_class("entry-url");
    // Opcional: Si quieres que la caja sea muy ancha, fuerza el tamaño:
    m_EntryUrl.set_size_request(400, -1);

    // 4. Radio Buttons (Opciones de límite)
    m_RadioProfundidad.set_label("Límite por Profundidad");
    m_RadioPaginas.set_label("Límite por Máx. Páginas");
    
    // Agruparlos (En GTK4 se usa set_group en el segundo radio)
    m_RadioPaginas.set_group(m_RadioProfundidad);
    m_RadioProfundidad.set_active(true); // Seleccionar el primero por defecto

    // Contenedor para los radios (para que estén uno al lado del otro o vertical)
    m_RadioBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_RadioBox.set_spacing(5);
    m_RadioBox.set_margin_bottom(15);
    m_RadioBox.append(m_RadioProfundidad);
    m_RadioBox.append(m_RadioPaginas);

    // 5. Botones de Acción
    m_BtnAnalizar.set_label("Analizar Sitio");
    // El CSS .menu-card button hará que se vea grande y cambie a rojo al pasar el mouse
    
    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("btn-salir"); // Usamos el estilo verde del botón salir

    // 6. Ensamblaje de la tarjeta
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInstruccion);
    m_CardBox.append(m_EntryUrl);
    m_CardBox.append(m_RadioBox);
    m_CardBox.append(m_BtnAnalizar);
    m_CardBox.append(m_BtnVolver);

    // 7. Centrar la tarjeta en la vista
    m_CenterBox.set_center_widget(m_CardBox);
    append(m_CenterBox);

    // 8. Conexión de Señales
    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VentanaSecundaria::on_volver_clicked));
    m_BtnAnalizar.signal_clicked().connect(sigc::mem_fun(*this, &VentanaSecundaria::on_analizar_clicked));
}

void VentanaSecundaria::on_volver_clicked() {
    // Regresar al índice 0 (Menú Principal)
    m_notebook.set_current_page(0);
}

// CORRECCIÓN: Asegúrate de que el nombre de la clase esté antes del método
void VentanaSecundaria::errorOcurrido(const std::string& mensaje) {
    // En GTK4 el constructor de MessageDialog es un poco diferente
    auto dialog = new Gtk::MessageDialog(mensaje, false, Gtk::MessageType::ERROR, Gtk::ButtonsType::OK, true);
    
    // IMPORTANTE: Obtener la ventana raíz para que el diálogo sea modal
    auto toplevel = dynamic_cast<Gtk::Window*>(this->get_root());
    if (toplevel) {
        dialog->set_transient_for(*toplevel);
    }

    dialog->signal_response().connect([dialog](int response_id) {
        delete dialog; // Limpieza de memoria
    });
    
    dialog->show();
}

// CORRECCIÓN: Solo una versión de este método
void VentanaSecundaria::on_analizar_clicked() {
    std::string url = m_EntryUrl.get_text();
    
    // 1. Validación de campo vacío
    if(url.empty()){
        errorOcurrido("No se ingresó ninguna URL, por favor intente nuevamente.");
        return;
    }

    // 2. Lógica de Petición
    // Nota: 'peticion' debe estar declarada en Interfaz.h dentro de VentanaSecundaria
    int resultado = peticion.realizarPeticion(url);

    if (resultado == 0){
        errorOcurrido("El dominio ingresado es inexistente, por favor intente nuevamente.");
        return;
    }

    if (resultado == 400){
        errorOcurrido("Error HTTP en el acceso al dominio, por favor ingrese otra URL.");
        return;
    }

    if (resultado == 200){
        peticion.guardarInformacion();
        
        auto dialog = new Gtk::MessageDialog("Análisis completado con éxito", false, 
                                            Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
        
        auto toplevel = dynamic_cast<Gtk::Window*>(this->get_root());
        if (toplevel) dialog->set_transient_for(*toplevel);

        dialog->signal_response().connect([dialog](int id){ delete dialog; });
        dialog->show();
    }
}
// =========================================================
// IMPLEMENTACIÓN VENTANA PRINCIPAL (INTERFAZ)
// =========================================================

Interfaz::Interfaz() {
    set_title("Web Crawler");
    set_default_size(900, 900);

    // 1. CARGA DE CSS (INTACTO)
    const std::string ESTILO_CSS = R"(
        window, notebook, stack { 
            background-color: #222223; 
        }

        .menu-card { 
            background-color: #BCB4FF; 
            border-radius: 15px; 
            padding: 80px; 
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
            font-size: 28px;   /* Tamaño de letra */
            font-weight: bold;
            color: #222223;
            margin-bottom: 15px; /* Separación con el input */
        }

        /* Clase para el campo de texto (Entry) */
        .entry-url {
            font-size: 20px;    /* Tamaño del texto que escribes */
            padding: 15px;      /* Hace la caja más "gorda" (relleno interno) */
            border-radius: 8px; /* Bordes redondeados */
            color: black;
            background-color: white;
            border: 2px solid #8bc34a; /* Un borde verde para que resalte */
        }
    )";

    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_data(ESTILO_CSS);

    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(), 
        css_provider, 
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    // 2. CONFIGURACIÓN DEL NOTEBOOK
    m_Notebook.set_scrollable(true);
    m_Notebook.set_show_tabs(true);
    m_Notebook.set_show_border(false);

    // --- PESTAÑA 0: INICIO ---
    m_titulo_principal.set_text("MENÚ");
    m_titulo_principal.add_css_class("titulo-label");

    m_MenuContainer.add_css_class("menu-card");
    m_MenuContainer.set_orientation(Gtk::Orientation::VERTICAL); // Importante: Vertical
    m_MenuContainer.set_spacing(10);
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

    // --- PESTAÑAS SECUNDARIAS ---
    
    // Creamos las instancias (Gestión de memoria automática con make_managed)
    auto* pagina1 = Gtk::make_managed<VentanaSecundaria>("Recopilación", m_Notebook);
    auto* pagina2 = Gtk::make_managed<VentanaSecundaria>("Búsqueda", m_Notebook);
    auto* pagina3 = Gtk::make_managed<VentanaSecundaria>("Análisis", m_Notebook);
    auto* pagina4 = Gtk::make_managed<VentanaSecundaria>("Enlaces", m_Notebook);

    m_Notebook.append_page(*pagina1, "Recopilar");
    m_Notebook.append_page(*pagina2, "Buscar");
    m_Notebook.append_page(*pagina3, "Análisis");
    m_Notebook.append_page(*pagina4, "Enlaces");

    // Señales del Menú Principal
    m_Btn1.signal_clicked().connect([this]{ m_Notebook.set_current_page(1); });
    m_Btn2.signal_clicked().connect([this]{ m_Notebook.set_current_page(2); });
    m_Btn3.signal_clicked().connect([this]{ m_Notebook.set_current_page(3); });
    m_Btn4.signal_clicked().connect([this]{ m_Notebook.set_current_page(4); });
    
    m_BtnSalir.signal_clicked().connect(sigc::mem_fun(*this, &Interfaz::on_salir_clicked));

    set_child(m_Notebook);
}

void Interfaz::on_salir_clicked() {
    hide(); 
}