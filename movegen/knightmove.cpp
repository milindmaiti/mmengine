#include "../utility/macros.h"
// mask all knight moves (color doesn't matter for knights)
ull mask_knight_attacks(int square) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);
  // moves towards the eigth rank
  attacks |= (bitboard << 6) & NO_HG_FILE;
  attacks |= (bitboard << 10) & NO_AB_FILE;
  attacks |= (bitboard << 15) & NO_H_FILE;
  attacks |= (bitboard << 17) & NO_A_FILE;

  // moves towards the first rank
  attacks |= (bitboard >> 6) & NO_AB_FILE;
  attacks |= (bitboard >> 10) & NO_HG_FILE;
  attacks |= (bitboard >> 15) & NO_A_FILE;
  attacks |= (bitboard >> 17) & NO_H_FILE;

  return attacks;
}
