#include "Board.h"
#include <iostream>

Board::Board() {
    setupBoard();
}

void Board::setupBoard()
{
    // clear board
    for (int r=0; r<SIZE; ++r)
        for (int c=0; c<SIZE; ++c)
            board[r][c] = Piece(PieceType::EMPTY, PieceColor::NONE);

    // ----- pawns ---------------------------------------------------------
    for (int c = 0; c < SIZE; ++c) {
        board[6][c] = Piece(PieceType::PAWN, PieceColor::WHITE); // row 6 = rank 2
        board[1][c] = Piece(PieceType::PAWN, PieceColor::BLACK); // row 1 = rank 7
    }

    // ----- major pieces --------------------------------------------------
    // White (bottom, row 7 / rank 1)
    board[7][0] = board[7][7] = Piece(PieceType::ROOK,   PieceColor::WHITE);
    board[7][1] = board[7][6] = Piece(PieceType::KNIGHT, PieceColor::WHITE);
    board[7][2] = board[7][5] = Piece(PieceType::BISHOP, PieceColor::WHITE);
    board[7][3]               = Piece(PieceType::QUEEN,  PieceColor::WHITE);
    board[7][4]               = Piece(PieceType::KING,   PieceColor::WHITE);

    // Black (top, row 0 / rank 8)
    board[0][0] = board[0][7] = Piece(PieceType::ROOK,   PieceColor::BLACK);
    board[0][1] = board[0][6] = Piece(PieceType::KNIGHT, PieceColor::BLACK);
    board[0][2] = board[0][5] = Piece(PieceType::BISHOP, PieceColor::BLACK);
    board[0][3]               = Piece(PieceType::QUEEN,  PieceColor::BLACK);
    board[0][4]               = Piece(PieceType::KING,   PieceColor::BLACK);

    sideToMove = PieceColor::WHITE;   // white begins
}


void Board::display() {
    std::cout << "  a b c d e f g h\n";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < SIZE; ++j) {
            std::cout << board[i][j].getSymbol() << " ";
        }
        std::cout << 8 - i << std::endl;
    }
    std::cout << "  a b c d e f g h\n";
}

// --- helpers -------------------------------------------------------------
static inline bool sameColor(const Piece& a,const Piece& b)
{ return a.getColor()!=PieceColor::NONE && a.getColor()==b.getColor(); }

bool Board::isPathClear(int fx,int fy,int tx,int ty) const
{
    int dx = (tx > fx) - (tx < fx);    // -1,0,1
    int dy = (ty > fy) - (ty < fy);
    for (int x=fx+dx, y=fy+dy; x!=tx || y!=ty; x+=dx, y+=dy)
        if (board[x][y].getType()!=PieceType::EMPTY)
            return false;
    return true;
}


// --- individual piece rules ---------------------------------------------
bool Board::legalPawn(int fx,int fy,int tx,int ty) const
{
    const Piece& p = board[fx][fy];
    int dir  = (p.getColor()==PieceColor::WHITE) ? -1 : 1;   // white moves up
    int start = (p.getColor()==PieceColor::WHITE) ? 6 : 1;

    // single step
    if (ty==fy && tx==fx+dir && board[tx][ty].getType()==PieceType::EMPTY)
        return true;

    // double step from initial rank
    if (ty==fy && fx==start && tx==fx+2*dir &&
        board[fx+dir][fy].getType()==PieceType::EMPTY &&
        board[tx][ty].getType()==PieceType::EMPTY)
        return true;

    // capture
    if (std::abs(ty-fy)==1 && tx==fx+dir &&
        board[tx][ty].getColor()!=PieceColor::NONE &&
        board[tx][ty].getColor()!=p.getColor())
        return true;

    return false;           // (promotion / en-passant handled later)
}

bool Board::legalKnight(int fx,int fy,int tx,int ty) const
{
    int dx=std::abs(tx-fx), dy=std::abs(ty-fy);
    return (dx==2 && dy==1) || (dx==1 && dy==2);
}

bool Board::legalBishop(int fx,int fy,int tx,int ty) const
{
    return std::abs(tx-fx)==std::abs(ty-fy) && isPathClear(fx,fy,tx,ty);
}

bool Board::legalRook(int fx,int fy,int tx,int ty) const
{
    return (fx==tx || fy==ty) && isPathClear(fx,fy,tx,ty);
}

bool Board::legalQueen(int fx,int fy,int tx,int ty) const
{
    return legalBishop(fx,fy,tx,ty) || legalRook(fx,fy,tx,ty);
}

bool Board::legalKing(int fx,int fy,int tx,int ty) const
{
    int dx=std::abs(tx-fx), dy=std::abs(ty-fy);
    return dx<=1 && dy<=1;      // (castling later)
}


bool Board::isLegalMove(int fx,int fy,int tx,int ty) const
{
    if (fx<0||fx>=SIZE||fy<0||fy>=SIZE||tx<0||tx>=SIZE||ty<0||ty>=SIZE)
        return false;

    const Piece& src = board[fx][fy];
    if (src.getColor()==PieceColor::NONE)
        return false;                                 // empty source

    if (sameColor(src, board[tx][ty]))                // cannot land atop own
        return false;

    switch (src.getType())
    {
        case PieceType::PAWN:   return legalPawn  (fx,fy,tx,ty);
        case PieceType::KNIGHT: return legalKnight(fx,fy,tx,ty);
        case PieceType::BISHOP: return legalBishop(fx,fy,tx,ty);
        case PieceType::ROOK:   return legalRook  (fx,fy,tx,ty);
        case PieceType::QUEEN:  return legalQueen (fx,fy,tx,ty);
        case PieceType::KING:   return legalKing  (fx,fy,tx,ty);
        default: return false;
    }
}

bool Board::kingCheckCoords(int& kx,int& ky) const
{
    PieceColor side = sideToMove;
    // find king
    for (int r=0;r<SIZE;++r)
        for (int c=0;c<SIZE;++c)
            if (board[r][c].getType()==PieceType::KING &&
                board[r][c].getColor()==side)
            { kx=r; ky=c; return kingInCheck(side); }
    return false;                    // should never happen
}

bool Board::squareAttacked(int x,int y,PieceColor by) const
{
    // scan every piece of 'by' colour and see if it can capture (ignoring own king safety)
    for (int r=0;r<SIZE;++r)
        for (int c=0;c<SIZE;++c)
            if (board[r][c].getColor()==by)
            {
                switch (board[r][c].getType())
                {
                    case PieceType::PAWN:   if (legalPawn  (r,c,x,y)) return true; break;
                    case PieceType::KNIGHT: if (legalKnight(r,c,x,y)) return true; break;
                    case PieceType::BISHOP: if (legalBishop(r,c,x,y)) return true; break;
                    case PieceType::ROOK:   if (legalRook  (r,c,x,y)) return true; break;
                    case PieceType::QUEEN:  if (legalQueen (r,c,x,y)) return true; break;
                    case PieceType::KING:   if (legalKing  (r,c,x,y)) return true; break;
                    default: break;
                }
            }
    return false;
}

bool Board::kingInCheck(PieceColor side) const
{
    // locate king
    int kx=-1,ky=-1;
    for (int r=0;r<SIZE && kx==-1;++r)
        for (int c=0;c<SIZE;++c)
            if (board[r][c].getType()==PieceType::KING &&
                board[r][c].getColor()==side)
            { kx=r; ky=c; break; }

    if (kx==-1) return false;   // should not happen

    PieceColor enemy = (side==PieceColor::WHITE)?PieceColor::BLACK:PieceColor::WHITE;
    return squareAttacked(kx,ky,enemy);
}

bool Board::wouldLeaveKingInCheck(int fx,int fy,int tx,int ty) const
{
    Board tmp = *this;                    // cheap copy (64 squares)
    tmp.board[tx][ty] = tmp.board[fx][fy];
    tmp.board[fx][fy] = Piece(PieceType::EMPTY, PieceColor::NONE);
    return tmp.kingInCheck(sideToMove);   // same side to move after hypothetical
}


// --- main movePiece ------------------------------------------------------
bool Board::movePiece(const std::string& from, const std::string& to)
{
    int fx = 8 - (from[1] - '0');
    int fy = from[0] - 'a';
    int tx = 8 - (to[1] - '0');
    int ty = to[0] - 'a';

    if (fx<0||fx>=SIZE||fy<0||fy>=SIZE||tx<0||tx>=SIZE||ty<0||ty>=SIZE)
        return false;

    const Piece& src = board[fx][fy];
    if (src.getColor()!=sideToMove)                    // wrong side
        return false;

    if (sameColor(src, board[tx][ty]))                 // own piece there
        return false;

    if (!isLegalMove(fx,fy,tx,ty))               return false; // pseudo-legal
    if (wouldLeaveKingInCheck(fx,fy,tx,ty))      return false; // safety

    // --- make the move ---
    board[tx][ty] = src;
    board[fx][fy] = Piece(PieceType::EMPTY, PieceColor::NONE);
    sideToMove = (sideToMove==PieceColor::WHITE) ? PieceColor::BLACK
                                                 : PieceColor::WHITE;
    return true;
}