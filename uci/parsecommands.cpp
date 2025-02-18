#include "../bitboard/bitboard.h"
#include "../search/negamax.h"
#include "../utility/boardnotation.h"
#include "../utility/macros.h"
#include "../utility/timerutility.h"
#include "parsefen.h"
#include <cctype>
#include <chrono>
#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <thread>

std::string moveToUciMove(int move) {
  int moveSrc = decode_src(move);
  int moveDst = decode_dst(move);
  int movePromotion = decode_promotion(move);
  std::string uciString = indexToSquare[moveSrc] + indexToSquare[moveDst];
  if (movePromotion > 0) {
    uciString += std::tolower(asciiPieces[movePromotion]);
  }
  return uciString;
}
int parse_move(Game &game, std::string &uciMove) {
  auto moveList = game.generate_moves();
  copy_current_board();
  for (int move : moveList) {
    if (!game.makeMove(move, false))
      continue;
    pop_current_copy();
    if (uciMove == moveToUciMove(move))
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

  size_t pos = command.find("moves", commandPtr);
  if (pos != std::string::npos) {
    // skip to actual moves
    commandPtr = pos + 6;
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
  /*print_board(game);*/
}

// Refactor Method to use threads for move timing
void parse_go(Game &game, std::string &command, Engine &engine) {
  int depth = -1;
  size_t commandPtr = 0;
  // skip over the go keyword
  commandPtr += 3;
  iterativeReturn ret;
  std::thread searchThread;
  if (command.substr(commandPtr, 5) == "wtime") {
    int wtime = 0, btime = 0, winc = 0, binc = 0;
    auto readNum = [&](std::string match, int &updateVar) {
      if (command.substr(commandPtr, match.size()) == match) {
        commandPtr += match.size() + 1;
        updateVar =
            stoi(command.substr(commandPtr, command.size() - commandPtr),
                 &commandPtr);
      }
    };
    // read in time that white has, and update the commandPtr to point to the
    // new location
    readNum("wtime", wtime);
    readNum("btime", btime);
    readNum("winc", winc);
    readNum("binc", binc);

    // TODO: Come up with more sophisticated time management mechanisms
    // currently spending 2% of time on each move
    // percentage of time to spend on a move
    double timePercent = 0.02;
    depth = 100;
    std::chrono::milliseconds movetime;
    if (game.side == white) {
      movetime = std::chrono::milliseconds((int)(wtime * timePercent));
    } else {
      movetime = std::chrono::milliseconds((int)(btime * timePercent));
    }

    auto stopFlag = std::make_shared<std::atomic<bool>>(false);
    engine.stopFlag = stopFlag;
    std::thread timerThread(threadTimer, movetime, stopFlag);
    std::thread searchThread(&Engine::searchPosition, &engine, std::ref(game),
                             depth, std::ref(ret));
    // once timerThread returns it sets stopFlag to true, stopping search thread
    timerThread.join();
    searchThread.join();
  } else if (command.substr(commandPtr, 5) == "depth") {
    // we are not using a timer so set stop flag to null
    engine.stopFlag = nullptr;
    commandPtr += 6;
    // parse depth
    depth = stoi(command.substr(commandPtr, command.size() - commandPtr));
    std::thread searchThread(&Engine::searchPosition, &engine, std::ref(game),
                             depth, std::ref(ret));
    searchThread.join();
  } else if (command.substr(commandPtr, 8) == "movetime") {
    commandPtr += 9;
    // some large depth value which will never finish to simulate running until
    // stopping
    depth = 100;
    // parse time in milliseconds
    std::chrono::milliseconds movetime(
        stoi(command.substr(commandPtr, command.size() - commandPtr)));
    // a shared ptr to a boolean flag with which the timer tells the search
    // thread to stop
    auto stopFlag = std::make_shared<std::atomic<bool>>(false);
    engine.stopFlag = stopFlag;
    std::thread timerThread(threadTimer, movetime, stopFlag);
    std::thread searchThread(&Engine::searchPosition, &engine, std::ref(game),
                             depth, std::ref(ret));
    // once timerThread returns it sets stopFlag to true, stopping search thread
    timerThread.join();
    searchThread.join();
  }
  for (int i = 0; i < (int)ret.evals.size(); i++) {
    std::cout << "info score cp " << ret.evals[i] << " depth " << i + 1
              << " nodes " << ret.nodeCounts[i] << " pv ";
    for (int j = 0; j <= i; j++) {
      std::cout << moveToUciMove(ret.pvs[i][j]) << " ";
    }
    std::cout << "\n";
  }

  std::cout << "\n";
  std::cout << "bestmove " << moveToUciMove(engine.pvTable[0][0]) << std::endl;
}

void uciLoop(Game &game, Engine &engine) {
  // turn off buffering for input and output streams
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);

  while (true) {
    std::string input;
    getline(std::cin, input);
    // no need to check for string length since substr will stop at end of
    // string
    if (input.substr(0, 2) == "go") {
      parse_go(game, input, engine);
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
