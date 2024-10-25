#include "movegen/bishopmove.h"
#include "utility/boardnotation.h"
#include "utility/macros.h"
#include <vector>

/* PRE-CALCULATED PAWN ATTACK TABLES */
ull pawnAttacks[NUM_SQ][NUM_SIDES];
/* PRE-CALCULATED KNIGHT ATTACK TABLES */
ull knightAttacks[NUM_SQ];
/* PRE-CALCULATED KING ATTACK TABLES */
ull kingAttacks[NUM_SQ];

// iterates over all subsets of a occupancy mask to prepare it for sliding piece
// attack tables
ull set_occupancy(int index, ull mask) {
  int count = 0;
  ull bitboard = 0;
  // iterate over set bits
  while (mask) {
    // get the index of that bit and pop it
    int lsbIndex = __builtin_ctzll(mask);
    pop_bit(mask, lsbIndex);

    // if the corresponding bit in index is set then set it in the new bitboard
    if (index & (1 << count))
      bitboard |= (1ULL << lsbIndex);
    count++;
  }
  return bitboard;
}
// Fills the pre-calculated attack tables for leaper pieces
void init_leaper_attacks() {
  for (int square = 0; square < NUM_SQ; square++) {
    for (auto side = 0; side < NUM_SIDES; side++) {
      pawnAttacks[square][side] = mask_pawn_attacks(square, side);
    }
    knightAttacks[square] = mask_knight_attacks(square);
    kingAttacks[square] = mask_king_attacks(square);
  }
}

// generate 64 bit random numbers using the XOR shift method

ull find_magic_number(int square, int relevantBits, int isBishop) {
  // Maximum amount of squares in a file or rank which could be blocker squares
  int maxRelevantSquares = 6;
  // Minimum bits in hashed numbers needed to avoid too frequent collisions
  int minHash = 6;
  // Need to consider combos of files and ranks.
  int attack_size = 1 << (2 * maxRelevantSquares);
  // extract top 8 bits from ull
  ull extract = 0xFF00000000000000;
  std::vector<ull> attacks(attack_size);
  std::vector<ull> used_attacks(attack_size);
  std::vector<ull> occupancies(attack_size);

  ull attack_mask =
      isBishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

  int occupancy_indices = (1 << relevantBits);
  // Iterate through different subsets of the occupancy bits and instantiate
  // both the occupancy bits and the valid squares to move to with the current
  // mask
  for (int i = 0; i < occupancy_indices; i++) {
    occupancies[i] = set_occupancy(i, attack_mask);
    attacks[i] = isBishop ? generate_bishop_moves_fly(square, occupancies[i])
                          : generate_rook_moves_fly(square, occupancies[i]);
  }

  ull magicNumber;
  while (true) {
    magicNumber = generate_random_number();
    bool currentMagicWorks = true;
    if (__builtin_popcount((magicNumber * attack_mask) & extract) < minHash)
      continue;
    for (int i = 0; i < occupancy_indices; i++) {
      int hash = (magicNumber * attack_mask) >> (64 - relevantBits);
      if (!used_attacks[hash]) {
        used_attacks[hash] = attacks[i];
      } else {
        if (used_attacks[hash] != attacks[i]) {
          currentMagicWorks = false;
          break;
        }
      }
    }
    if (currentMagicWorks)
      break;
  }
  return magicNumber;
}
int main() {
  init_leaper_attacks();
  find_magic_number(e4, , int isBishop)
  /*for (int i = 0; i < 4096; i++) {*/
  /*  print_bitboard(set_occupancy(i, mask_rook_attacks(a1)));*/
  /*  getchar();*/
  /*}*/
}
