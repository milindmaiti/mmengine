#ifndef BITBOARD
#define BITBOARD

#include "utility/macros.h"
ull maskPawnAttacks(int square, int side);
ull maskKnightAttacks(int square);
ull maskKingAttacks(int square);
ull mask_bishop_attacks(int square);
ull mask_rook_attacks(int square);
ull generate_bishop_moves_fly(int square, ull mask);
ull generate_rook_moves_fly(int square, ull mask);
ull set_occupancy(int index, ull mask);
void init_leaper_attacks();
ull generate_random_number();
#endif
