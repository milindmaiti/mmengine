#include "utility/macros.h"
// generate bishop moves considering blockers
ull generate_bishop_moves_fly(int square, ull mask) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  BitUtil::set_bit(bitboard, square);

  int rank = (square >> 3);
  int file = (square % Constants::BD);

  for (ull curRank = rank + 1, curFile = file + 1;
       curRank <= Constants::BD - 1 && curFile <= Constants::BD - 1;
       curRank++, curFile++) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
  }

  for (ull curRank = rank + 1, curFile = file - 1;
       curRank <= Constants::BD - 1 && curFile >= 0; curRank++, curFile--) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
    if (mask & (1ULL << ((curRank << 3) + curFile)))
      break;
  }

  for (ull curRank = rank - 1, curFile = file + 1;
       curRank >= 0 && curFile <= Constants::BD - 1; curRank--, curFile++) {
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
  BitUtil::set_bit(bitboard, square);

  int rank = (square >> 3);
  int file = (square % Constants::BD);

  for (ull curRank = rank + 1, curFile = file + 1;
       curRank < Constants::BD - 1 && curFile < Constants::BD - 1;
       curRank++, curFile++) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (ull curRank = rank + 1, curFile = file - 1;
       curRank < Constants::BD - 1 && curFile > 0; curRank++, curFile--) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (ull curRank = rank - 1, curFile = file + 1;
       curRank > 0 && curFile < Constants::BD - 1; curRank--, curFile++) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  for (ull int curRank = rank - 1, curFile = file - 1;
       curRank > 0 && curFile > 0; curRank--, curFile--) {
    attacks |= (1ULL << ((curRank << 3) + curFile));
  }

  return attacks;
}

ull get_bishop_attack(
    int square, ull occupancyMask,
    std::array<ull, Constants::NUM_SQ> &bishopMasks,
    std::array<std::array<ull, 1 << Constants::MAX_BISHOP_BLOCK>,
               Constants::NUM_SQ> &bishopAttacks,
    std::array<ull, Constants::NUM_SQ> &magics) {
  // take only the blocker bits from the mask
  occupancyMask &= bishopMasks[square];
  return bishopAttacks[square][(magics[square] * occupancyMask) >>
                               (Constants::NUM_SQ -
                                ArrayUtil::bishopRelevantBits[square])];
}
