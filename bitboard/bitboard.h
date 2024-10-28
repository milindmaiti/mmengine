#ifndef BITBOARD
#define BITBOARD

#include "../utility/macros.h"
ull set_occupancy(int index, ull mask);
void init_leaper_attacks();
ull find_magic_number(int square, int relevantBits, int isBishop);
#endif
