cmake_minimum_required(VERSION 3.16)

project(ImageCC VERSION 0.1 LANGUAGES C CXX)

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

set(Qt6_DIR "C:/Qt/6.9.0/msvc2022_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)


file(GLOB_RECURSE CPP_SRC src/gui/*.cpp src/gui/*.c)
file(GLOB_RECURSE C_SRC src/logic/*.cpp src/logic/*.c)
file(GLOB_RECURSE HEADERS  include/*.h include/*.hpp)

add_executable(ImageCC ${CPP_SRC} ${C_SRC} ${HEADERS})

target_include_directories(ImageCC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(QTNODES STATIC IMPORTED GLOBAL)

add_definitions(-DNODE_EDITOR_SHARED)
set_target_properties(QTNODES PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/QtNodes.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/QtNodes"
)

target_link_libraries(ImageCC
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    QTNODES
)


include(GNUInstallDirs)
install(TARGETS ImageCC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ImageCC)
endif()

add_custom_command(TARGET ImageCC POST_BUILD
    COMMAND "C:/Qt/6.9.0/msvc2022_64/bin/windeployqt.exe"
    --dir $<TARGET_FILE_DIR:ImageCC>
    $<TARGET_FILE:ImageCC>
    COMMENT "Running windeployqt to deploy Qt dependencies"
)
add_custom_command(TARGET ImageCC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/6.9.0/msvc2022_64/bin/Qt6Core.dll"
    $<TARGET_FILE_DIR:ImageCC>
    COMMENT "Copying Qt6Core.dll"
)
add_custom_command(TARGET ImageCC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/6.9.0/msvc2022_64/bin/Qt6Gui.dll"
    $<TARGET_FILE_DIR:ImageCC>
    COMMENT "Copying Qt6Gui.dll"
)
add_custom_command(TARGET ImageCC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Qt/6.9.0/msvc2022_64/bin/Qt6Widgets.dll"
    $<TARGET_FILE_DIR:ImageCC>
    COMMENT "Copying Qt6Widgets.dll"
)
add_custom_command(TARGET ImageCC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/QtNodes.dll"
    $<TARGET_FILE_DIR:ImageCC>
    COMMENT "Copying Qt6Widgets.dll"
)
