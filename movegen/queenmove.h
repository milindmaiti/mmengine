#pragma once
#include "../utility/macros.h"
#include "./bishopmove.h"
#include "./rookmove.h"
// returns a mask of squares a queen can move to given other piece placement
inline ull get_queen_attack(
    int square, ull occupancyMask, std::array<ull, NUM_SQ> &rookMasks,
    std::array<std::array<ull, 1 << MAX_ROOK_BLOCK>, NUM_SQ> &rookAttacks,
    std::array<ull, NUM_SQ> &rookMagicTable,
    std::array<ull, NUM_SQ> &bishopMasks,
    std::array<std::array<ull, 1 << MAX_BISHOP_BLOCK>, NUM_SQ> &bishopAttacks,
    std::array<ull, NUM_SQ> &bishopMagicTable) {
  return (get_rook_attack(square, occupancyMask, rookMasks, rookAttacks,
                          rookMagicTable) |
          get_bishop_attack(square, occupancyMask, bishopMasks, bishopAttacks,
                            bishopMagicTable));
}
