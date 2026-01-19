#pragma once
#include <gtkmm.h>
#include <string>
#include <negocios/Peticion.h>


// =========================================================
// 1. CLASE VISTA RECOPILACIÓN (Con Radios y URL)
// =========================================================
class VistaRecopilacion : public Gtk::Box {
public:
    VistaRecopilacion(Gtk::Notebook& notebook);
protected:
    void on_volver_clicked();
    void on_analizar_clicked();
    Gtk::Notebook& m_notebook;
    
    Gtk::CenterBox m_CenterBox;
    Gtk::Box m_CardBox, m_RadioBox;
    Gtk::Label m_LblTitulo, m_LblInstruccion;
    Gtk::Entry m_EntryUrl;
    Gtk::CheckButton m_RadioProfundidad, m_RadioPaginas; // SOLO AQUI
    Gtk::Button m_BtnAnalizar, m_BtnVolver;
};

// =========================================================
// 2. CLASE VISTA BÚSQUEDA (Sin Radios, pide Palabra Clave)
// =========================================================
class VistaBusqueda : public Gtk::Box {
public:
    VistaBusqueda(Gtk::Notebook& notebook);
protected:
    void on_volver_clicked();
    void on_buscar_clicked();
    Gtk::Notebook& m_notebook;

    Gtk::CenterBox m_CenterBox;
    Gtk::Box m_CardBox;
    Gtk::Label m_LblTitulo, m_LblInstruccion;
    Gtk::Entry m_EntryKeyword; // Entrada para palabra clave
    Gtk::Entry m_EntryUrl;
    Gtk::Button m_BtnBuscar, m_BtnVolver;
};

// =========================================================
// 3. CLASE VISTA ANÁLISIS (Sin Radios, solo URL)
// =========================================================
class VistaAnalisis : public Gtk::Box {
public:
    VistaAnalisis(Gtk::Notebook& notebook);
protected:
    void on_volver_clicked();
    void on_analizar_clicked();
    Gtk::Notebook& m_notebook;

    Gtk::CenterBox m_CenterBox;
    Gtk::Box m_CardBox;
    Gtk::Label m_LblTitulo, m_LblInstruccion;
    Gtk::Entry m_EntryUrl; 
    Gtk::Button m_BtnAnalizar, m_BtnVolver;
};

// =========================================================
// 4. CLASE VISTA ENLACES (Solo muestra botón de carga/lista)
// =========================================================
class VistaEnlaces : public Gtk::Box {
public:
    VistaEnlaces(Gtk::Notebook& notebook);
protected:
    void on_volver_clicked();
    void on_mostrar_clicked();
    Gtk::Notebook& m_notebook;

    Gtk::CenterBox m_CenterBox;
    Gtk::Box m_CardBox;
    Gtk::Label m_LblTitulo, m_LblInfo;
    Gtk::Button m_BtnMostrar, m_BtnVolver;
};

// =========================================================
// CLASE PRINCIPAL
// =========================================================
class Interfaz : public Gtk::Window {
public:
    Interfaz();
protected:
    void on_salir_clicked();
    Gtk::Notebook m_Notebook;
    Gtk::CenterBox m_CenterBoxInicio;
    Gtk::Box m_MenuContainer;
    Gtk::Label m_titulo_principal;
    Gtk::Button m_Btn1, m_Btn2, m_Btn3, m_Btn4, m_BtnSalir;
};

