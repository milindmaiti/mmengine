#pragma once
#include "boardstate.h"
#include "movegen/bishopmove.h"
#include "movegen/rookmove.h"
#include "utility/macros.h"
#include <vector>
class Game {
public:
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
  std::array<ull, Constants::NUM_BITBOARDS> pieceBitboards;
  std::array<ull, Constants::NUM_OCCUPANCIES> occupancyBitboards;
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
  std::array<std::array<ull, Constants::NUM_SIDES>, Constants::NUM_SQ>
      pawnAttacks;
  /* pre-calculated knight attack tables */
  std::array<ull, Constants::NUM_SQ> knightAttacks;
  /* pre-calculated king attack tables */
  std::array<ull, Constants::NUM_SQ> kingAttacks;

  // rook attack masks (considering no blockers)
  std::array<ull, Constants::NUM_SQ> rookMasks;
  // bishop attack masks (considering no blockers)
  std::array<ull, Constants::NUM_SQ> bishopMasks;

  // rook attack masks (indexed by square and blocker piece masks)
  std::array<std::array<ull, 1 << Constants::MAX_ROOK_BLOCK>, Constants::NUM_SQ>
      rookAttacks;
  // bishop attack masks (indexed by square and blocker piece masks)
  std::array<std::array<ull, 1 << Constants::MAX_BISHOP_BLOCK>,
             Constants::NUM_SQ>
      bishopAttacks;

  std::array<ull, Constants::NUM_SQ> rookMagics;
  std::array<ull, Constants::NUM_SQ> bishopMagics;

  Game(const std::array<ull, Constants::NUM_SQ> &rookMagics,
       const std::array<ull, Constants::NUM_SQ> &bishopMagics,
       int side = Notation::white, int enPassant = -1, int castle = 15,
       int halfMoves = 0, int fullMoves = 0,
       std::array<ull, Constants::NUM_BITBOARDS> pieceBitboards = {},
       std::array<ull, Constants::NUM_OCCUPANCIES> occupancyBitboards = {});
  Game(int side = Notation::white, int enPassant = -1, int castle = 15,
       int halfMoves = 0, int fullMoves = 0,
       std::array<ull, Constants::NUM_BITBOARDS> pieceBitboards = {},
       std::array<ull, Constants::NUM_OCCUPANCIES> occupancyBitboards = {});
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
        pieceBitboards[(side == Notation::white) ? Notation::P : Notation::p])
      return true;
    if (knightAttacks[square] &
        pieceBitboards[(side == Notation::white) ? Notation::N : Notation::n])
      return true;
    if (kingAttacks[square] &
        pieceBitboards[(side == Notation::white) ? Notation::K : Notation::k])
      return true;
    if (get_bishop_attack(square, occupancyBitboards[Notation::both],
                          this->bishopMasks, this->bishopAttacks,
                          this->bishopMagics) &
        (pieceBitboards[(side == Notation::white) ? Notation::B : Notation::b] |
         pieceBitboards[(side == Notation::white) ? Notation::Q : Notation::q]))
      return true;
    if (get_rook_attack(square, occupancyBitboards[Notation::both],
                        this->rookMasks, this->rookAttacks, this->rookMagics) &
        (pieceBitboards[(side == Notation::white) ? Notation::R : Notation::r] |
         pieceBitboards[(side == Notation::white) ? Notation::Q : Notation::q]))
      return true;

    return false;
  }
  void inline loop_attacks(int sourceSquare, ull attackBitboard, int piece,
                           std::vector<int> &moveList);
  std::vector<int> generate_moves();
  int makeMove(int move, bool onlyCapture);
  void init_all();

  BoardState saveState() const;
  void restoreState(const BoardState &state);
};
