#pragma once
#include "../bitboard/bitboard.h"
#include "../utility/macros.h"
ull perftDriver(Game &game, ull &captures, ull &eps, ull &castles,
                ull &promotions, int depth);
