#ifndef PIECE_H
#define PIECE_H

enum class PieceType {
    EMPTY,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

enum class PieceColor {
    NONE,
    WHITE,
    BLACK
};

class Piece {
public:
    Piece(PieceType type = PieceType::EMPTY, PieceColor color = PieceColor::NONE);

    PieceType getType() const;
    PieceColor getColor() const;
    char getSymbol() const;

private:
    PieceType type;
    PieceColor color;
};

#endif // PIECE_H
