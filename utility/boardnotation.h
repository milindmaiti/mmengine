#pragma once
#include "../bitboard/bitboard.h"
#include "macros.h"

void printNoQuotes();
void printQuotes();
void print_bitboard(ull bitboard);
void printBoard(Game &game);
void printMoves(std::vector<int> &moveList);
void printMove(int move);
