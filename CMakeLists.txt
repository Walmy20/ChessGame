cmake_minimum_required(VERSION 3.28)
project(ChessGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GLFW3
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/imgui)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)


# Source files
set(SOURCES
    src/main.cpp
    src/chess/Board.cpp
    src/chess/Piece.cpp
    src/ui/ImGuiHandler.cpp
)

# Add executable
add_executable(ChessGame ${SOURCES})

# ImGui integration
target_include_directories(ChessGame PRIVATE
    ${CMAKE_SOURCE_DIR}/src/chess
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/backends
)

target_link_libraries(ChessGame PRIVATE imgui glfw OpenGL::GL)
