#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/macros.h"
#include "parsefen.h"
#include <cctype>
#include <cstdio>
#include <sstream>
#include <string>

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
  if (command.substr(commandPtr, 8) == "startpos") {
    parse_fen(game, startPosition);
    commandPtr += 9;
  } else if (command.substr(commandPtr, 3) == "fen") {
    commandPtr += 4;
    parse_fen(game, command.substr(commandPtr, command.size() - commandPtr));
  } else {
    parse_fen(game, startPosition);
  }

  if (commandPtr < (int)command.size() &&
      command.substr(commandPtr, 5) == "moves") {
    // skip to actual moves
    commandPtr += 6;
    std::stringstream moveStream(
        command.substr(commandPtr, command.size() - commandPtr));

    std::string uciMove;
    while (moveStream >> uciMove) {
      int curMove = parse_move(game, uciMove);
      if (curMove == 0)
        break;
      game.makeMove(curMove, false);
    }
  }
  print_board(game);
}

void parse_go(Game &game, std::string &command) {
  int depth = -1;
  int commandPtr = 0;
  // skip over the go keyword
  commandPtr += 3;
  if (command.substr(commandPtr, 5) == "depth") {
    commandPtr += 6;
    depth = stoi(command.substr(commandPtr, command.size() - commandPtr));
  }
  std::cout << depth << "\n";
  if (depth != -1)
    game.search_position(depth);
}

void uciLoop(Game &game) {
  // turn off buffering for input and output streams
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  std::cout << "id name mmengine" << std::endl;
  std::cout << "id author Milind" << std::endl;
  std::cout << "uciok" << std::endl;

  while (true) {
    std::string input;
    getline(std::cin, input);
    // no need to check for string length since substr will stop at end of
    // string
    if (input.substr(0, 2) == "go") {
      parse_go(game, input);
    } else if (input.substr(0, 7) == "isready") {
      std::cout << "readyok" << std::endl;
    } else if (input.substr(0, 8) == "position") {
      parse_position(game, input);
    } else if (input.substr(0, 10) == "ucinewgame") {
      std::string command = "position startpos";
      parse_position(game, command);
    } else if (input.substr(0, 4) == "quit") {
      break;
    } else if (input.substr(0, 3) == "uci") {
      std::cout << "id name mmengine" << std::endl;
      std::cout << "id author Milind" << std::endl;
      std::cout << "uciok" << std::endl;
    }
  }
}
