#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "Piece.h"


constexpr int SIZE = 8;

class Board {
private:
    Piece board[SIZE][SIZE];
    PieceColor sideToMove = PieceColor::WHITE;

    bool isPathClear(int fx,int fy,int tx,int ty)          const;
    bool legalPawn  (int fx,int fy,int tx,int ty)          const;
    bool legalKnight(int fx,int fy,int tx,int ty)          const;
    bool legalBishop(int fx,int fy,int tx,int ty)          const;
    bool legalRook  (int fx,int fy,int tx,int ty)          const;
    bool legalQueen (int fx,int fy,int tx,int ty)          const;
    bool legalKing  (int fx,int fy,int tx,int ty)          const;
    
    


public:
    Board();
    void setupBoard();
    void display();
    bool movePiece(const std::string& from, const std::string& to);
    bool isLegalMove(int fx,int fy,int tx,int ty)   const;   
    const Piece& getPiece(int row, int col) const { return board[row][col]; }
    PieceColor turn() const { return sideToMove; }      
    bool kingCheckCoords(int& kx,int& ky) const; 
    bool squareAttacked(int x,int y,PieceColor by)         const;
    bool kingInCheck   (PieceColor side)                   const;
    bool wouldLeaveKingInCheck(int fx,int fy,int tx,int ty) const;    
    
};

#endif // BOARD_H