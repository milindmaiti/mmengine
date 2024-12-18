#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/macros.h"
#include "parsefen.h"
#include <cctype>
#include <sstream>

int parse_move(Game &game, std::string &uciMove) {
  auto moveList = game.generate_moves();
  copy_current_board();
  for (int move : moveList) {
    if (!game.makeMove(move, false))
      continue;
    pop_current_copy();
    int moveSrc = decode_src(move);
    int moveDst = decode_dst(move);
    std::string uciString = indexToSquare[moveSrc] + indexToSquare[moveDst];
    if (uciMove.size() == 5) {
      int movePromotion = decode_promotion(move);
      if (movePromotion == 0) {
        continue;
      } else {
        uciString += std::tolower(asciiPieces[movePromotion]);
      }
    }

    if (uciString == uciMove)
      return move;
  }
  return 0;
}

void parse_position(Game &game, std::string &command) {
  int commandPtr = 0;
  // skip the "position" word and whitespace
  commandPtr += 9;
  if (command.substr(commandPtr, 8) == "startPos") {
    parse_fen(game, startPosition);
  } else {
    if (command.substr(commandPtr, 3) == "fen") {
      commandPtr += 4;
      parse_fen(game, command.substr(commandPtr, command.size() - commandPtr));
    } else {
      if (command.substr(commandPtr, 5) == "moves") {
        // skip to actual moves
        commandPtr += 6;
        std::stringstream moveStream(
            command.substr(commandPtr, command.size() - commandPtr));

        std::string uciMove;
        while (moveStream >> uciMove) {
          int curMove = parse_move(game, uciMove);
          if (curMove == 0)
            return;
          game.makeMove(curMove, false);
        }
      } else {
        return;
      }
    }
  }
  print_board();
}
