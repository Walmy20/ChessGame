#pragma once
#include <GLFW/glfw3.h>
#include "Board.h"

class ImGuiHandler {
public:
    explicit ImGuiHandler(Board& boardRef);        // pass board by ref

    void initialize(GLFWwindow* window);
    void newFrame();
    void render();                                 // draws board & handles clicks
    void renderUI();                               // ends frame / presents
    void cleanup();

private:
    Board& board;
    int    selRow = -1;                            // first click state
    int    selCol = -1;
};