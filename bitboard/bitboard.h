#pragma once
#include "../movegen/bishopmove.h"
#include "../movegen/kingmove.h"
#include "../movegen/knightmove.h"
#include "../movegen/pawnmove.h"
#include "../movegen/queenmove.h"
#include "../movegen/rookmove.h"
#include "../utility/macros.h"
#include <vector>
class Game {
public:
#define copy_board()                                                           \
  std::array<ull, NUM_BITBOARDS> copyPieceBitboards = pieceBitboards;          \
  std::array<ull, NUM_OCCUPANCIES> copyOccupancyBitboards =                    \
      occupancyBitboards;                                                      \
  int copySide = side;                                                         \
  int copyEnPassant = enPassant;                                               \
  int copyCastle = castle;                                                     \
  int copyHalfMoves = halfMoves;                                               \
  int copyFullMoves = fullMoves;

#define pop_copy()                                                             \
  pieceBitboards = copyPieceBitboards;                                         \
  occupancyBitboards = copyOccupancyBitboards;                                 \
  side = copySide;                                                             \
  enPassant = copyEnPassant;                                                   \
  castle = copyCastle;                                                         \
  halfMoves = copyHalfMoves;                                                   \
  fullMoves = copyFullMoves;
  /*
   * ---- BOARD REPRESENTATION ----
   * We will have 12 pieceBitboard for pieces
   *  - King, Queen, Rook, Knight, Bishop, Pawn for each side
   * We will have 3 for just entire board
   * - One with only black pieces, one with only white, one with all the pieces
   *
   * ---- CASTLING ----
   * wk, wq, bk, bq
   *
   * 0001 - white kingside
   * 0010 - white queenside
   * 0100 - black kingside
   * 1000 - black queenside
   */
  std::array<ull, NUM_BITBOARDS> pieceBitboards;
  std::array<ull, NUM_OCCUPANCIES> occupancyBitboards;
  // side to move
  int side;
  // en passant capture square
  int enPassant;
  // castling rights
  int castle;

  // number of half moves made since last capture or pawn advance (for 50 move
  // rule)
  int halfMoves;

  // number of full moves made since last capture or pawn advance
  int fullMoves;

  /* pre-calculated pawn attack tables */
  std::array<std::array<ull, NUM_SIDES>, NUM_SQ> pawnAttacks;
  /* pre-calculated knight attack tables */
  std::array<ull, NUM_SQ> knightAttacks;
  /* pre-calculated king attack tables */
  std::array<ull, NUM_SQ> kingAttacks;

  // rook attack masks (considering no blockers)
  std::array<ull, NUM_SQ> rookMasks;
  // bishop attack masks (considering no blockers)
  std::array<ull, NUM_SQ> bishopMasks;

  // rook attack masks (indexed by square and blocker piece masks)
  std::array<std::array<ull, 1 << MAX_ROOK_BLOCK>, NUM_SQ> rookAttacks;
  // bishop attack masks (indexed by square and blocker piece masks)
  std::array<std::array<ull, 1 << MAX_BISHOP_BLOCK>, NUM_SQ> bishopAttacks;

  std::array<ull, NUM_SQ> rookMagics;
  std::array<ull, NUM_SQ> bishopMagics;

  Game(std::array<ull, NUM_SQ> rookMagics, std::array<ull, NUM_SQ> bishopMagics,
       int side = white, int enPassant = -1, int castle = 15, int halfMoves = 0,
       int fullMoves = 0, std::array<ull, NUM_BITBOARDS> pieceBitboards = {},
       std::array<ull, NUM_OCCUPANCIES> occupancyBitboards = {});
  Game(int side = white, int enPassant = -1, int castle = 15, int halfMoves = 0,
       int fullMoves = 0, std::array<ull, NUM_BITBOARDS> pieceBitboards = {},
       std::array<ull, NUM_OCCUPANCIES> occupancyBitboards = {});
  // iterates over all subsets of a occupancy mask to prepare it for sliding
  // piece attack tables
  ull set_occupancy(int index, ull mask);

  ull find_magic_number(int square, int relevantBits, int isBishop);
  // Fills the pre-calculated attack tables for leaper pieces
  void init_leaper_attacks();

  void init_magic_numbers();

  void init_slider_attacks(int isBishop);

  /**
   * Checks to see if a square is attacked by side. It works by generating moves
   * by different pieces of the opposite side's color, and then checking if the
   * current side has any pieces there. For example, if the square is e3 and
   * side is white, then the function will simulate a black pawn on e3, knight
   * on e3, etc. and then get the squares that the piece is attacking (i.e. g2,
   * c2, d1, etc. for the black knight). it will then check if there is a white
   * piece on those squares, and return true if so. For slider pieces like
   * rooks, it also takes into account the full occupancyBitboard
   */
  inline ull is_square_attacked(int square, int side) {

    int oppositeSide = 1 - side;
    if (pawnAttacks[square][oppositeSide] &
        pieceBitboards[(side == white) ? P : p])
      return true;
    if (knightAttacks[square] & pieceBitboards[(side == white) ? N : n])
      return true;
    if (kingAttacks[square] & pieceBitboards[(side == white) ? K : k])
      return true;
    if (get_bishop_attack(square, occupancyBitboards[both], this->bishopMasks,
                          this->bishopAttacks, this->bishopMagics) &
        (pieceBitboards[(side == white) ? B : b] |
         pieceBitboards[(side == white) ? Q : q]))
      return true;
    if (get_rook_attack(square, occupancyBitboards[both], this->rookMasks,
                        this->rookAttacks, this->rookMagics) &
        (pieceBitboards[(side == white) ? R : r] |
         pieceBitboards[(side == white) ? Q : q]))
      return true;

    return false;
  }
  void inline loop_attacks(int sourceSquare, ull attackBitboard, int piece,
                           std::vector<int> &moveList);
  std::vector<int> generate_moves();
  int makeMove(int move, bool onlyCapture);
  void init_all();
};
