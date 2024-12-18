#pragma once
#include "../bitboard/bitboard.h"
#include "macros.h"

void print_no_quotes();
void print_quotes();
void print_bitboard(ull bitboard);
void print_board(Game &game);
void print_moves(std::vector<int> &moveList);
void print_move(int move);
