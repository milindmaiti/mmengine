#include "../utility/macros.h"
// mask all knight moves (color doesn't matter for knights)
ull mask_knight_attacks(int square) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  BitUtil::set_bit(bitboard, square);
  // moves towards the eigth rank
  attacks |= (bitboard << 6) & Constants::NO_HG_FILE;
  attacks |= (bitboard << 10) & Constants::NO_AB_FILE;
  attacks |= (bitboard << 15) & Constants::NO_H_FILE;
  attacks |= (bitboard << 17) & Constants::NO_A_FILE;

  // moves towards the first rank
  attacks |= (bitboard >> 6) & Constants::NO_AB_FILE;
  attacks |= (bitboard >> 10) & Constants::NO_HG_FILE;
  attacks |= (bitboard >> 15) & Constants::NO_A_FILE;
  attacks |= (bitboard >> 17) & Constants::NO_H_FILE;

  return attacks;
}
