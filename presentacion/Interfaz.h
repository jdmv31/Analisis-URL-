#pragma once
#include <gtkmm.h>
#include <string>
#include <negocios/Peticion.h>

// =========================================================
// 1. CLASE VISTA RECOPILACIÓN
// =========================================================
class VistaRecopilacion : public Gtk::Box {
public:
    VistaRecopilacion(Gtk::Notebook& notebook);
protected:
    void on_volver_clicked();
    void on_analizar_clicked();
    void on_guardar_clicked();
    void on_cancelar_resultados_clicked();

    Gtk::Notebook& m_notebook;
    Gtk::CenterBox m_CenterBox;

    // --- VISTA 1: INPUT ---
    Gtk::Box m_CardBox, m_RadioBox;
    Gtk::Label m_LblTitulo, m_LblInstruccion;
    Gtk::Label m_LblError; 
    Gtk::Entry m_EntryUrl;
    // Fusion: Agregamos el EntryLimite de tu compañero
    Gtk::Entry m_EntryLimite;
    Gtk::CheckButton m_RadioProfundidad, m_RadioPaginas;
    Gtk::Button m_BtnAnalizar, m_BtnVolver;

    // --- VISTA 2: RESULTADOS ---
    Gtk::Box m_CardBoxResultados;
    Gtk::Label m_LblResTitulo, m_LblResUrl;
    Gtk::TextView m_TxtInfoContenido;
    Gtk::ScrolledWindow m_ScrollInfo;
    Gtk::Button m_BtnGuardar, m_BtnDescartar;
};

// =========================================================
// 2. CLASE VISTA BÚSQUEDA
// =========================================================
class VistaBusqueda : public Gtk::Box {
public:
    VistaBusqueda(Gtk::Notebook& notebook);
protected:
    void on_volver_clicked();
    void on_buscar_clicked();
    void on_nueva_busqueda_clicked();
    bool validar_texto(const std::string& texto); 

    Gtk::Notebook& m_notebook;
    Gtk::CenterBox m_CenterBox;

    // Fusion: Mantenemos tu estructura y agregamos los controles de él
    Gtk::Box m_CardBox, m_RadioBox;
    Gtk::Label m_LblTitulo, m_LblInstruccion, m_LblUrl, m_LblError;
    Gtk::Entry m_EntryKeyword, m_EntryUrl;
    Gtk::Entry m_EntryLimite; // De tu compañero
    Gtk::Button m_BtnBuscar, m_BtnVolver;

    Gtk::Box m_CardBoxResultados;
    Gtk::Label m_LblResTitulo, m_LblResDato;
    Gtk::TextView m_TxtResContenido;
    Gtk::ScrolledWindow m_ScrollRes;
    Gtk::Button m_BtnNuevaBusqueda, m_BtnIrMenu;
};

// =========================================================
// 3. CLASE VISTA ANÁLISIS (MODIFICADA)
// =========================================================
class VistaAnalisis : public Gtk::Box {
public:
    VistaAnalisis(Gtk::Notebook& notebook);
protected:
    void on_volver_clicked();
    void on_analizar_clicked();
    void on_cerrar_resultados_clicked(); 

    Gtk::Notebook& m_notebook;
    Gtk::CenterBox m_CenterBox;

    // --- VISTA 1: INPUT ---
    Gtk::Box m_CardBox;
    Gtk::Label m_LblTitulo, m_LblInfo; // Agregamos m_LblInfo
    Gtk::Label m_LblError; 
    // Eliminamos m_EntryUrl
    Gtk::Button m_BtnAnalizar, m_BtnVolver;

    // --- VISTA 2: RESULTADOS ---
    Gtk::Box m_CardBoxResultados;
    Gtk::Label m_LblResTitulo;
    Gtk::Label m_LblHuerfanas, m_LblImagenes, m_LblPromedio, m_LblVisitadas;
    Gtk::Button m_BtnCerrarResultado;
};

// =========================================================
// 4. CLASE VISTA ENLACES (ACTUALIZADA)
// =========================================================
class VistaEnlaces : public Gtk::Box {
public:
    VistaEnlaces(Gtk::Notebook& notebook);
protected:
    void on_volver_clicked();
    void on_mostrar_clicked();
    void on_cerrar_lista_clicked(); 

    Gtk::Notebook& m_notebook;
    Gtk::CenterBox m_CenterBox;

    // --- VISTA 1: MENÚ ---
    Gtk::Box m_CardBox;
    Gtk::Label m_LblTitulo, m_LblInfo;
    Gtk::Button m_BtnMostrar, m_BtnVolver;
    Gtk::Label m_LblError;

    // --- VISTA 2: LISTA DE ENLACES ---
    Gtk::Box m_CardBoxResultados;
    Gtk::Label m_LblResTitulo;
    Gtk::TextView m_TxtResContenido; 
    Gtk::ScrolledWindow m_ScrollRes; 
    Gtk::Button m_BtnCerrarLista;
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