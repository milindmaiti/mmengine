#include "../utility/macros.h"
// generate bishop moves considering blockers
ull generate_bishop_moves_fly(int square, ull mask) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);

  int rank = (square >> 3);
  int file = (square % BD);

  for (int curRank = rank + 1, curFile = file + 1;
       curRank < BD - 1 && curFile < BD - 1; curRank++, curFile++) {
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (int curRank = rank + 1, curFile = file - 1;
       curRank < BD - 1 && curFile > 0; curRank++, curFile--) {
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (int curRank = rank - 1, curFile = file + 1;
       curRank > 0 && curFile < BD - 1; curRank--, curFile++) {
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (int curRank = rank - 1, curFile = file - 1; curRank > 0 && curFile > 0;
       curRank--, curFile--) {
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  return attacks;
}
// produce all squares that a bishop can move to given its square
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
