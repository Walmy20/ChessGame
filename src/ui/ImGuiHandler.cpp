// ImGuiHandler.cpp -------------------------------------------------------
#include "ImGuiHandler.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

ImGuiHandler::ImGuiHandler(Board& b) : board(b) {}

void ImGuiHandler::initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.Fonts->AddFontDefault();            // default font is fine for now

    static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x2654, 0x265F, 0 };

    io.Fonts->AddFontFromFileTTF(
            "../assets/DejaVuSans.ttf",
            32.0f,          // looks good for 64-px squares
            nullptr,        // ImFontConfig – nullptr = default
            ranges);

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

static const char* pieceUtf8(const Piece& p)
{
    switch (p.getType()) {
        case PieceType::KING:   return (p.getColor()==PieceColor::WHITE) ? u8"\u2654" : u8"\u265A";
        case PieceType::QUEEN:  return (p.getColor()==PieceColor::WHITE) ? u8"\u2655" : u8"\u265B";
        case PieceType::ROOK:   return (p.getColor()==PieceColor::WHITE) ? u8"\u2656" : u8"\u265C";
        case PieceType::BISHOP: return (p.getColor()==PieceColor::WHITE) ? u8"\u2657" : u8"\u265D";
        case PieceType::KNIGHT: return (p.getColor()==PieceColor::WHITE) ? u8"\u2658" : u8"\u265E";
        case PieceType::PAWN:   return (p.getColor()==PieceColor::WHITE) ? u8"\u2659" : u8"\u265F";
        default:                return " ";          // empty square
    }
}

void ImGuiHandler::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


static std::string idxToSquare(int row, int col)   // helper a1-h8
{
    return { char('a'+col), char('8'-row) };
}

void ImGuiHandler::render()
{
    ImGuiViewport* vp = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);

    const ImGuiWindowFlags flags =
          ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize    | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
        | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::Begin("##FullscreenBoard", nullptr, flags);

    // ── board metrics ────────────────────────────────────────────────────
    const float SQ      = 64.0f;                         // size of a square
    const float gap     = ImGui::GetStyle().ItemSpacing.x;
    const float boardPx = 8*SQ + 7*gap;                  // full width/height

    ImVec2 winPos  = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();

    const ImVec2 boardOrigin = {
        winPos.x + (winSize.x - boardPx) * 0.5f,
        winPos.y + (winSize.y - boardPx) * 0.5f
    };

    int checkX=-1, checkY=-1;
    bool isCheck = board.kingCheckCoords(checkX, checkY);

    // ── draw 8×8 board ───────────────────────────────────────────────────
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(gap, gap));

    for (int r = 0; r < 8; ++r)
    {
        // reset X,Y so every rank starts at the same left edge
        ImVec2 rowStart = { boardOrigin.x, boardOrigin.y + r * (SQ + gap) };
        ImGui::SetCursorScreenPos(rowStart);

        for (int c = 0; c < 8; ++c)
        {
            ImGui::PushID(r*8 + c);

            //------------------------------------------------- colour selection begin
            bool isOrigin  = (r==selRow && c==selCol);
            bool isTarget  = (selRow!=-1) && !isOrigin &&
                            board.isLegalMove(selRow, selCol, r, c) &&
                            !board.wouldLeaveKingInCheck(selRow, selCol, r, c);
            bool isKingInCheck = isCheck && (r==checkX && c==checkY);
            bool lightSquare   = ((r+c)&1)==0;

            ImVec4 col =
                isKingInCheck                ? ImVec4(0.70f,0.20f,0.20f,1.0f) // red
                : isOrigin                     ? ImVec4(0.95f,0.85f,0.10f,1.0f) // yellow
                : isTarget                     ? ImVec4(0.20f,0.65f,0.25f,1.0f) // green
                : lightSquare                  ? ImVec4(0.85f,0.85f,0.85f,1.0f)
                                            : ImVec4(0.30f,0.30f,0.30f,1.0f);
            //------------------------------------------------- colour selection end
            if (r == selRow && c == selCol)
                col = ImVec4(0.95f,0.85f,0.10f,1);

            ImGui::PushStyleColor(ImGuiCol_Button,       col);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,col);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, col);

            const char* glyph = pieceUtf8(board.getPiece(r,c));
            if (ImGui::Button(glyph, ImVec2(SQ, SQ)))
            {
                if (selRow == -1) { selRow = r; selCol = c; }
                else {
                    board.movePiece(idxToSquare(selRow,selCol),
                                    idxToSquare(r,c));
                    selRow = selCol = -1;
                }
            }

            ImGui::PopStyleColor(3);
            if (c != 7) ImGui::SameLine(0.0f, gap);      // explicit gap
            ImGui::PopID();
        }
    }

    ImGui::PopStyleVar();          // ItemSpacing
    ImGui::End();
    ImGui::PopStyleVar();          // WindowPadding
}



void ImGuiHandler::renderUI()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHandler::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
