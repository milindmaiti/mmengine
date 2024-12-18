#pragma once
#include "../bitboard/bitboard.h"
void parse_position(Game &game, std::string &command);
int parse_move(Game &game, std::string &uciMove);
void uciLoop(Game &game);
