#pragma once
#include "utility/macros.h"
ull generate_bishop_moves_fly(int square, ull mask);
ull mask_bishop_attacks(int square);

// returns a mask of squares a bishop can move to given other piece placement
ull get_bishop_attack(
    int square, ull occupancyMask,
    std::array<ull, Constants::NUM_SQ> &bishopMasks,
    std::array<std::array<ull, 1 << Constants::MAX_BISHOP_BLOCK>,
               Constants::NUM_SQ> &bishopAttacks,
    std::array<ull, Constants::NUM_SQ> &magics);
