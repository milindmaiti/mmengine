#include "../bitboard/bitboard.h"
#include "../utility/genspecialboards.h"
#include "../utility/macros.h"
#include "../utility/parsefen.h"
#include "../utility/perft.h"
int main() {
  Game chess(rookMagics, bishopMagics);
  std::string curFen = "8/8/3p4/KPp4r/1R3p1k/8/4P1P1/8 w - c6 0 1";
  parse_fen(chess, curFen);
  ull captures = 0, eps = 0, castles = 0, promotions = 0;
  ull nodes = perftDriver(chess, captures, eps, castles, promotions, 1);
  std::cout << nodes << "\n";
  auto moves = chess.generate_moves();
  print_moves(moves);
}
