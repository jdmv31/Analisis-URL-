cmake_minimum_required(VERSION 3.15)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(cpr CONFIG REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
pkg_check_modules(GUMBO REQUIRED IMPORTED_TARGET gumbo)

add_library(CapaDatos STATIC datos/Ficheros.cpp)
target_include_directories(CapaDatos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(CapaNegocios STATIC negocios/Peticion.cpp)
target_include_directories(CapaNegocios PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CapaNegocios PRIVATE cpr::cpr PkgConfig::GUMBO)
add_definitions(-D_HAS_STD_BYTE=0)
add_library(CapaInterfaz STATIC presentacion/Interfaz.cpp)
target_include_directories(CapaInterfaz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CapaInterfaz PRIVATE PkgConfig::GTKMM)
add_executable(WebCrawler main.cpp)


target_link_libraries(WebCrawler PRIVATE 
    CapaNegocios
    CapaInterfaz
    CapaDatos
    PkgConfig::GTKMM
)