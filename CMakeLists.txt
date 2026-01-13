cmake_minimum_required(VERSION 3.15)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(cpr CONFIG REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
pkg_check_modules(GUMBO REQUIRED IMPORTED_TARGET gumbo)


# --- CAPA DATOS ---

# 1. Limpiamos la ruta
file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/datos" RUTA_LIMPIA)

# 2. Creamos un archivo 'PathConfig.h' automáticamente en la carpeta de compilación
#    Esto escribe: #define RUTA_DATOS "C:/Tu/Ruta/datos"
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/PathConfig.h" 
     "#define RUTA_DATOS \"${RUTA_LIMPIA}\"")

add_library(CapaDatos STATIC datos/Fichero.cpp)

# 3. YA NO usamos target_compile_definitions para la ruta. BORRA LO ANTERIOR.
# target_compile_definitions(...) <- Elimina esto si aún lo tienes

# 4. Incluimos la carpeta donde se creó el archivo PathConfig.h (BINARY_DIR)
target_include_directories(CapaDatos PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

add_library(CapaNegocios STATIC 
    negocios/Peticion.cpp 
    negocios/Cola.cpp
)
target_include_directories(CapaDatos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CapaNegocios PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CapaNegocios 
    PRIVATE cpr::cpr PkgConfig::GUMBO
    PUBLIC CapaDatos)
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