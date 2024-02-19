cmake_minimum_required(VERSION 3.25.1)

project(Earth VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt5_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR Dependencies/qtimgui/include/qtimgui
                Dependencies/qtimgui/include/imgui
                Dependencies/qtimgui/include/implot)
                
set(LIBS_DIR Dependencies/qtimgui/lib)

set(LIBS imgui implot qt_imgui_widgets)

find_package(Qt5 COMPONENTS Core Widgets OpenGL Gui REQUIRED)

add_compile_options(/bigobj)

file(GLOB SOURCES Sources/*.cpp Sources/*.h *.qrc)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" ${INCLUDE_DIR})

link_directories(${LIBS_DIR})

add_executable(Earth ${SOURCES})

target_link_libraries(Earth Qt5::Core Qt5::Widgets Qt5::OpenGL ${LIBS})

add_custom_command(TARGET Earth
    POST_BUILD COMMAND xcopy /E /Y 
    "\"$(SolutionDir)../Resources\""
    "\"$(OutDir)Resources/*\""
)

add_custom_command(TARGET Earth
    POST_BUILD COMMAND xcopy /E /Y 
    "\"$(SolutionDir)../Resources\""
    "\"$(BuildDir)Resources/*\""
)

add_custom_command(TARGET Earth
    POST_BUILD COMMAND "$ENV{Qt5_DIR}/bin/windeployqt.exe"
    "\"$(OutDir)$(TargetName)$(TargetExt)\""
)