#include "../bitboard/bitboard.h"
#include "../search/negamax.h"
#include "../uci/parsecommands.h"
#include "../utility/evaluationtables.h"
#include "../utility/genspecialboards.h"
#include "../utility/macros.h"
#include "../utility/parsefen.h"
#include "../utility/perft.h"
#include "../utility/timerutility.h"
#include <csignal>
#include <cstdio>

using std::cout;
int getCapturedPiece2(int move, Game &game) {
  int lowerPiece, higherPiece;
  int dst = decode_dst(move);

  if (game.side == white) {
    lowerPiece = p, higherPiece = q;
  } else {
    lowerPiece = P, higherPiece = Q;
  }
  for (int capturedPiece = lowerPiece; capturedPiece <= higherPiece;
       capturedPiece++) {
    if (get_bit(game.pieceBitboards[capturedPiece], dst)) {
      return capturedPiece;
    }
  }
  return -1;
};
int main() {
  Game chess(rookMagics, bishopMagics);
  Engine engine(128);

  /*parse_fen(chess, trickyPosition);*/
  /*ull time1 = getMilliseconds();*/
  /*auto ret = engine.searchPosition(chess, 6);*/
  /*ull time2 = getMilliseconds();*/
  /*cout << "Time Elapsed: " << time2 - time1 << "\n";*/
  // testing speed of different methods
  /*parse_fen(chess, killerPosition);*/
  /*print_board(chess);*/
  /*ull startTime = getMilliseconds();*/
  /*int depth = 5;*/
  /*iterativeReturn ret = engine.searchPosition(chess, depth);*/
  /*ull endTime = getMilliseconds();*/
  /*cout << "Time Taken: " << (double)(endTime - startTime) / 1000 << "s\n";*/
  /**/
  /*for (int i = 0; i < (int)ret.evals.size(); i++) {*/
  /*  std::cout << "info score cp " << ret.evals[i] << " depth " << i + 1*/
  /*            << " nodes " << ret.nodeCounts[i] << " pv ";*/
  /*  for (int j = 0; j <= i; j++) {*/
  /*    std::cout << moveToUciMove(ret.pvs[i][j]) << " ";*/
  /*  }*/
  /*  std::cout << "\n";*/
  /*}*/
  /**/
  /*parse_fen(chess, cmkPosition);*/
  /*print_board(chess);*/
  /*startTime = getMilliseconds();*/
  /*ret = engine.searchPosition(chess, depth);*/
  /*endTime = getMilliseconds();*/
  /*cout << "Time Taken: " << (double)(endTime - startTime) / 1000 << "s\n";*/
  /**/
  /*for (int i = 0; i < (int)ret.evals.size(); i++) {*/
  /*  std::cout << "info score cp " << ret.evals[i] << " depth " << i + 1*/
  /*            << " nodes " << ret.nodeCounts[i] << " pv ";*/
  /*  for (int j = 0; j <= i; j++) {*/
  /*    std::cout << moveToUciMove(ret.pvs[i][j]) << " ";*/
  /*  }*/
  /*  std::cout << "\n";*/
  /*}*/
  /*std::string pos =*/
  /*    "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0
   * 9";*/
  /*parse_fen(chess, pos);*/
  /*auto moveList = chess.generate_moves();*/
  /*sort(moveList.begin(), moveList.end(), [&](int move1, int move2) {*/
  /*  int cap1 = decode_capture(move1);*/
  /*  int cap2 = decode_capture(move2);*/
  /*  if (cap1 == 0 && cap2 == 0)*/
  /*    return false;*/
  /*  if (cap1 != 0 && cap2 == 0)*/
  /*    return true;*/
  /*  else if (cap2 != 0 && cap1 == 0)*/
  /*    return false;*/
  /*  else {*/
  /*    int piece1 = getCapturedPiece2(move1, chess);*/
  /*    int piece2 = getCapturedPiece2(move2, chess);*/
  /*    return mvvtable[decode_piece(move1)][piece1] >=*/
  /*           mvvtable[decode_piece(move2)][piece2];*/
  /*    return true;*/
  /*  }*/
  /*});*/
  /**/
  /*print_moves(moveList);*/
  /*cout << asciiPieces[decode_piece(moveList[1])] << " "*/
  /*     << mvvtable[decode_piece(moveList[1])]*/
  /*                [getCapturedPiece2(moveList[1], chess)]*/
  /*     << "\n";*/
  /*cout << asciiPieces[decode_piece(moveList[2])] << " "*/
  /*     << mvvtable[decode_piece(moveList[2])]*/
  /*                [getCapturedPiece2(moveList[2], chess)]*/
  /*     << "\n";*/
  /*print_board(chess);*/

  uciLoop(chess, engine);
  /*ull captures = 0, eps = 0, castles = 0, promotions = 0;*/
  /*ull nodes = perftDriver(chess, captures, eps, castles, promotions, 1);*/
  /*std::cout << nodes << "\n";*/
  /*auto moves = chess.generate_moves();*/
  /*print_moves(moves);*/
}
