#pragma once
#include "../bitboard/bitboard.h"
#include "../search/negamax.h"
void parse_position(Game &game, std::string &command);
int parse_move(Game &game, std::string &uciMove);
void uciLoop(Game &game, Engine &engine);
void parse_go(Game &game, std::string &command);
std::string moveToUciMove(int move);
