#pragma once
#include "utility/macros.h"

ull generate_rook_moves_fly(int square, ull mask);
ull mask_rook_attacks(int square);
// returns a mask of squares a rook can move to given other piece placement
ull get_rook_attack(int square, ull occupancyMask,
                    std::array<ull, Constants::NUM_SQ> &rookMasks,
                    std::array<std::array<ull, 1 << Constants::MAX_ROOK_BLOCK>,
                               Constants::NUM_SQ> &rookAttacks,
                    std::array<ull, Constants::NUM_SQ> &magics);
