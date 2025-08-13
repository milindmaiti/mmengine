#pragma once
#include "bishopmove.h"
#include "rookmove.h"
#include "utility/macros.h"
// returns a mask of squares a queen can move to given other piece placement
inline ull
get_queen_attack(int square, ull occupancyMask,
                 std::array<ull, Constants::NUM_SQ> &rookMasks,
                 std::array<std::array<ull, 1 << Constants::MAX_ROOK_BLOCK>,
                            Constants::NUM_SQ> &rookAttacks,
                 std::array<ull, Constants::NUM_SQ> &rookMagicTable,
                 std::array<ull, Constants::NUM_SQ> &bishopMasks,
                 std::array<std::array<ull, 1 << Constants::MAX_BISHOP_BLOCK>,
                            Constants::NUM_SQ> &bishopAttacks,
                 std::array<ull, Constants::NUM_SQ> &bishopMagicTable) {
  return (get_rook_attack(square, occupancyMask, rookMasks, rookAttacks,
                          rookMagicTable) |
          get_bishop_attack(square, occupancyMask, bishopMasks, bishopAttacks,
                            bishopMagicTable));
}
