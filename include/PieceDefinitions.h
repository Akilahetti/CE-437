#ifndef PIECE_DEFINITIONS_H
#define PIECE_DEFINITIONS_H

#include <map>

// Define NFC tag to piece name mappings
const std::map<uint8_t, String> pieceNames = {
    {0x1, "White Knight (King Side)"},
    {0x2, "White Knight (Queen Side)"},
    {0x3, "White Bishop (King Side)"},
    {0x4, "White Bishop (Queen Side)"},
    {0x5, "White Rook (King Side)"},
    {0x6, "White Rook (Queen Side)"},
    {0x7, "White Queen"},
    {0x8, "White King"},
    {0x9, "White Pawn A"},
    {0xA, "White Pawn B"},
    {0xB, "White Pawn C"},
    {0xC, "White Pawn D"},
    {0xD, "White Pawn E"},
    {0xE, "White Pawn F"},
    {0xF, "White Pawn G"},
    {0x10, "White Pawn H"},
    {0x11, "Black Knight (King Side)"},
    {0x12, "Black Knight (Queen Side)"},
    {0x13, "Black Bishop (King Side)"},
    {0x14, "Black Bishop (Queen Side)"},
    {0x15, "Black Rook (King Side)"},
    {0x16, "Black Rook (Queen Side)"},
    {0x17, "Black Queen"},
    {0x18, "Black King"},
    {0x19, "Black Pawn A"},
    {0x1A, "Black Pawn B"},
    {0x1B, "Black Pawn C"},
    {0x1C, "Black Pawn D"},
    {0x1D, "Black Pawn E"},
    {0x1E, "Black Pawn F"},
    {0x1F, "Black Pawn G"},
    {0x20, "Black Pawn H"}};

#endif // PIECE_DEFINITIONS_H
