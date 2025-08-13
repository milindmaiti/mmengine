#include "../utility/macros.h"
// generates all possible king attacks from a given square
ull mask_king_attacks(int square) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  BitUtil::set_bit(bitboard, square);
  // moves towards the eigth rank
  attacks |= (bitboard << 1) & Constants::NO_A_FILE;
  attacks |= (bitboard << 9) & Constants::NO_A_FILE;
  attacks |= (bitboard >> 7) & Constants::NO_A_FILE;

  attacks |= (bitboard << 8);
  attacks |= (bitboard >> 8);

  attacks |= (bitboard >> 1) & Constants::NO_H_FILE;
  attacks |= (bitboard >> 9) & Constants::NO_H_FILE;
  attacks |= (bitboard << 7) & Constants::NO_H_FILE;

  return attacks;
}
