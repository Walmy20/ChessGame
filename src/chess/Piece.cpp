#include "Piece.h"

Piece::Piece(PieceType type, PieceColor color) : type(type), color(color) {}

PieceType Piece::getType() const {
    return type;
}

PieceColor Piece::getColor() const {
    return color;
}

char Piece::getSymbol() const {
    switch (type) {
        case PieceType::PAWN:   return (color == PieceColor::WHITE) ? 'P' : 'p';
        case PieceType::ROOK:   return (color == PieceColor::WHITE) ? 'R' : 'r';
        case PieceType::KNIGHT: return (color == PieceColor::WHITE) ? 'N' : 'n';
        case PieceType::BISHOP: return (color == PieceColor::WHITE) ? 'B' : 'b';
        case PieceType::QUEEN:  return (color == PieceColor::WHITE) ? 'Q' : 'q';
        case PieceType::KING:   return (color == PieceColor::WHITE) ? 'K' : 'k';
        default:                return '.';
    }
}
