#pragma once
#include "../utility/macros.h"
ull generate_rook_moves_fly(int square, ull mask);
ull mask_rook_attacks(int square);
// returns a mask of squares a rook can move to given other piece placement
inline ull get_rook_attack(
    int square, ull occupancyMask, std::array<ull, NUM_SQ> &rookMasks,
    std::array<std::array<ull, 1 << MAX_ROOK_BLOCK>, NUM_SQ> &rookAttacks,
    std::array<ull, NUM_SQ> &magics) {
  // take only the blocker bits from the mask
  occupancyMask &= rookMasks[square];
  return rookAttacks[square][(magics[square] * occupancyMask) >>
                             (NUM_SQ - rookRelevantBits[square])];
}
