#include "utility/macros.h"
#include <array>

struct BoardState {
  std::array<ull, Constants::NUM_BITBOARDS> pieceBitboards;
  std::array<ull, Constants::NUM_OCCUPANCIES> occupancyBitboards;
  int side;
  int enPassant;
  int castle;
  int halfMoves;
  int fullMoves;
};
