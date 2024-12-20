#include "../utility/macros.h"
// generate rook moves with mask
ull generate_rook_moves_fly(int square, ull mask) {

  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);

  int rank = (square >> 3);
  int file = (square % BD);

  for (int curRank = rank + 1; curRank <= BD - 1; curRank++) {
    attacks |= (1ULL << ((curRank << 3) + file));
    if (mask & (1ULL << ((curRank << 3) + file)))
      break;
  }

  for (int curRank = rank - 1; curRank >= 0; curRank--) {
    attacks |= (1ULL << ((curRank << 3) + file));
    if (mask & (1ULL << ((curRank << 3) + file)))
      break;
  }
  for (int curFile = file + 1; curFile <= BD - 1; curFile++) {
    attacks |= (1ULL << ((rank << 3) + curFile));
    if (mask & (1ULL << ((rank << 3) + curFile)))
      break;
  }
  for (int curFile = file - 1; curFile >= 0; curFile--) {
    attacks |= (1ULL << ((rank << 3) + curFile));
    if (mask & (1ULL << ((rank << 3) + curFile)))
      break;
  }

  return attacks;
}

// generate all possible positions that a blocker piece could occupy which could
// affect the squares that the rook could travel to
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
