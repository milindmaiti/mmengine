#include "macros.h"
#include <array>
// clang-format off

// pawn positional score
extern const std::array<int, NUM_SQ> pawnScores = 
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
extern const std::array<int, NUM_SQ> knightScores =
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
extern const std::array<int, NUM_SQ> bishopScores =
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0
};

// rook positional score
extern const std::array<int, NUM_SQ> rookScores =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
extern const std::array<int, NUM_SQ> kingScores =
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,   5,   0, -15,   0,  10,   0
};

extern const std::array<std::array<int, NUM_PIECES>, NUM_PIECES> mvvtable = {{
  {105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605},
	{104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604},
  {	103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603},
  {	102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602},
{	101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601},
{	100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600},

{	105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605},
{	104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604},
{	103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603},
{	102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602},
{101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601},
{100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600}
}};

