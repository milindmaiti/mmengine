#include "utility/boardnotation.h"
#include "utility/macros.h"
#include <filesystem>
#include <iostream>

/* PRE-CALCULATED PAWN ATTACK TABLES */
ull pawnAttacks[NUM_SQ][NUM_SIDES];
/* PRE-CALCULATED KNIGHT ATTACK TABLES */
ull knightAttacks[NUM_SQ];
/* PRE-CALCULATED KING ATTACK TABLES */
ull kingAttacks[NUM_SQ];

// Creates masks for the squares that the pawns attack
ull maskPawnAttacks(int square, int side) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);

  if (side == white) {
    ull right_attacks = (bitboard >> 7) & NO_A_FILE;
    ull left_attacks = (bitboard >> 9) & NO_H_FILE;
    attacks |= right_attacks;
    attacks |= left_attacks;
  } else {
    ull right_attacks = (bitboard << 7) & NO_H_FILE;
    ull left_attacks = (bitboard << 9) & NO_A_FILE;
    attacks |= right_attacks;
    attacks |= left_attacks;
  }
  return attacks;
}

// mask all knight moves (color doesn't matter for knights)
ull maskKnightAttacks(int square) {
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

// generates all possible king attacks from a given square
ull maskKingAttacks(int square) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);
  // moves towards the eigth rank
  attacks |= (bitboard << 1) & NO_A_FILE;
  attacks |= (bitboard << 9) & NO_A_FILE;
  attacks |= (bitboard >> 7) & NO_A_FILE;

  attacks |= (bitboard << 8);
  attacks |= (bitboard >> 8);

  attacks |= (bitboard >> 1) & NO_H_FILE;
  attacks |= (bitboard >> 9) & NO_H_FILE;
  attacks |= (bitboard << 7) & NO_H_FILE;

  return attacks;
}

ull mask_bishop_attacks(int square) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);

  int rank = (square >> 3);
  int file = (square % BD);

  for (int curRank = rank + 1, curFile = file + 1;
       curRank < BD - 1 && curFile < BD - 1; curRank++, curFile++) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (int curRank = rank + 1, curFile = file - 1;
       curRank < BD - 1 && curFile > 0; curRank++, curFile--) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (int curRank = rank - 1, curFile = file + 1;
       curRank > 0 && curFile < BD - 1; curRank--, curFile++) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (int curRank = rank - 1, curFile = file - 1; curRank > 0 && curFile > 0;
       curRank--, curFile--) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  return attacks;
}

ull mask_rook_attacks(int square) {

  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);

  int rank = (square >> 3);
  int file = (square % BD);

  for (int curRank = rank + 1; curRank < BD - 1; curRank++) {
    attacks |= (1ULL << ((curRank << 3) + file));
  }

  for (int curRank = rank - 1; curRank > 0; curRank--) {
    attacks |= (1ULL << ((curRank << 3) + file));
  }
  for (int curFile = file + 1; curFile < BD - 1; curFile++) {
    attacks |= (1ULL << ((rank << 3) + curFile));
  }
  for (int curFile = file - 1; curFile > 0; curFile--) {
    attacks |= (1ULL << ((rank << 3) + curFile));
  }

  return attacks;
}
// Fills the pre-calculated attack tables for leaper pieces
void init_leaper_attacks() {
  for (int square = 0; square < NUM_SQ; square++) {
    for (auto side = 0; side < NUM_SIDES; side++) {
      pawnAttacks[square][side] = maskPawnAttacks(square, side);
    }
    knightAttacks[square] = maskKnightAttacks(square);
    kingAttacks[square] = maskKingAttacks(square);
  }
}

int main() {
  /*init_leaper_attacks();*/
  /*print_bitboard(maskKnightAttacks(a1));*/

  /*print_bitboard(mask_bishop_attacks(d5));*/
  print_bitboard(mask_rook_attacks(d4));
  /*print_bitboard(kingAttacks[f3]);*/
  /*print_bitboard(pawnAttacks[c8][black]);*/
  // bitboard representing all the squares
  /*ull bitboard = 0ULL;*/
  /*set_bit(bitboard, h1);*/
  /*print_bitboard(bitboard);*/
}
