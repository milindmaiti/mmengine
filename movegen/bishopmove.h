#pragma once
#include "../utility/macros.h"
ull generate_bishop_moves_fly(int square, ull mask);
ull mask_bishop_attacks(int square);

// returns a mask of squares a bishop can move to given other piece placement
inline ull get_bishop_attack(
    int square, ull occupancyMask, std::array<ull, NUM_SQ> &bishopMasks,
    std::array<std::array<ull, 1 << MAX_BISHOP_BLOCK>, NUM_SQ> &bishopAttacks,
    std::array<ull, NUM_SQ> &magics) {
  // take only the blocker bits from the mask
  occupancyMask &= bishopMasks[square];
  return bishopAttacks[square][(magics[square] * occupancyMask) >>
                               (NUM_SQ - bishopRelevantBits[square])];
}
