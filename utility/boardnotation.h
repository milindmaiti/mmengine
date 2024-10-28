#ifndef BOARDNOTATION
#define BOARDNOTATION
#include "macros.h"

void printNoQuotes();
void printQuotes();
void print_bitboard(ull bitboard);
void printBoard(ull *pieceBitboards, int side, int castle, int enpassant);
#endif
