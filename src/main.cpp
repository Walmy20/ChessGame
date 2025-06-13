#include <GLFW/glfw3.h>
#include "ui/ImGuiHandler.h"
#include "chess/Board.h"
#include <iostream>

int main()
{
    if (!glfwInit()) { std::cerr << "GLFW init failed\n"; return -1; }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Chess Game", nullptr, nullptr);
    if (!window) { std::cerr << "Window creation failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    Board         chessBoard;           // *** one board only ***
    ImGuiHandler  gui(chessBoard);
    gui.initialize(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();               // 0. events

        gui.newFrame();                 // 1-3. start ImGui frame
        gui.render();                   // 4. build UI (board + clicks)
        gui.renderUI();                 // 5-6. end ImGui frame

        glfwSwapBuffers(window);        // 7. present
    }

    gui.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
