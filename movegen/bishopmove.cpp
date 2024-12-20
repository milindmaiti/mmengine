#include "../utility/macros.h"
// generate bishop moves considering blockers
ull generate_bishop_moves_fly(int square, ull mask) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);

  int rank = (square >> 3);
  int file = (square % BD);

  for (int curRank = rank + 1, curFile = file + 1;
       curRank <= BD - 1 && curFile <= BD - 1; curRank++, curFile++) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
  }

  for (int curRank = rank + 1, curFile = file - 1;
       curRank <= BD - 1 && curFile >= 0; curRank++, curFile--) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
  }

  for (int curRank = rank - 1, curFile = file + 1;
       curRank >= 0 && curFile <= BD - 1; curRank--, curFile++) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
  }

  for (int curRank = rank - 1, curFile = file - 1; curRank >= 0 && curFile >= 0;
       curRank--, curFile--) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
  }

  return attacks;
}
// produce a mask of all possible blocker positions of a bishop that pieces
// could occupy
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
