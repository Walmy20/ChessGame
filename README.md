# ChessGame
Chess Game using ImGui &amp; glfw3 &amp; OpenGl

![image](https://github.com/user-attachments/assets/c4468b03-0b22-41ef-8930-1d24b6ba1e80)


```
ChessGame
├── src
│   ├── main.cpp               # Entry point of the application
│   ├── chess
│   │   ├── Board.cpp          # Implementation of the Board class
│   │   ├── Board.h            # Declaration of the Board class
│   │   ├── Piece.cpp          # Implementation of the Piece class
│   │   └── Piece.h            # Declaration of the Piece class
│   └── ui
│       ├── ImGuiHandler.cpp    # Implementation of the ImGuiHandler class
│       └── ImGuiHandler.h      # Declaration of the ImGuiHandler class
|
├── imgui
│   ├── imgui.h            # Main header file for ImGui
│   ├── imgui.cpp          # Implementation of ImGui
│   ├── imgui_draw.cpp     # Drawing functions for ImGui
│   ├── imgui_widgets.cpp   # Widget functions for ImGui
│   └── imgui_impl_glfw.cpp # Integration of ImGui with GLFW
│── backends
|
├── assets
│   
├── CMakeLists.txt             # CMake configuration file
└── README.md                  # Project documentation

```


## Setup Instructions

1. **Clone the repository**:
   ```
   git clone <repository-url>
   cd ChessGame
   ```

2. **Install dependencies**:
   Ensure you have CMake, GLFW, and the ImGui library installed on your system.

3. **Build the project**:
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the application**:
   ```
   ./ChessGame
   ```
