cmake_minimum_required(VERSION 3.16)

project(ImageC VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(
        /Zc:__cplusplus
        /permissive-
    )
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui)


file(GLOB_RECURSE CPP_SRC src/gui/*.cpp)
file(GLOB_RECURSE C_SRC src/logic/*.c)
file(GLOB_RECURSE HEADERS  include/*.h include/*.hpp)

add_executable(ImageC ${CPP_SRC} ${C_SRC} ${HEADERS})

target_include_directories(ImageC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(QTNODES STATIC IMPORTED GLOBAL)

add_definitions(-DNODE_EDITOR_SHARED)
set_target_properties(QTNODES PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/QtNodes.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/QtNodes"
)

target_link_libraries(ImageC
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    QTNODES
)


include(GNUInstallDirs)
install(TARGETS ImageC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ImageC)
endif()
