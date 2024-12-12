#include "bitboard.h"
#include "../movegen/bishopmove.h"
#include "../movegen/kingmove.h"
#include "../movegen/knightmove.h"
#include "../movegen/pawnmove.h"
#include "../movegen/queenmove.h"
#include "../movegen/rookmove.h"
#include "../utility/boardnotation.h"
#include "../utility/genspecialboards.h"
#include "../utility/macros.h"
#include "../utility/randomutility.h"
#include <array>

Game::Game(std::array<ull, NUM_SQ> rookMagics,
           std::array<ull, NUM_SQ> bishopMagics, int side, int enPassant,
           int castle, int halfMoves, int fullMoves,
           std::array<ull, NUM_BITBOARDS> pieceBitboards,
           std::array<ull, NUM_OCCUPANCIES> occupancyBitboards)
    : rookMagics(rookMagics), bishopMagics(bishopMagics), side(side),
      enPassant(enPassant), castle(castle), halfMoves(halfMoves),
      fullMoves(fullMoves), pieceBitboards(pieceBitboards),
      occupancyBitboards(occupancyBitboards) {
  init_leaper_attacks();
  init_slider_attacks(rook);
  init_slider_attacks(bishop);
}

Game::Game(int side, int enPassant, int castle, int halfMoves, int fullMoves,
           std::array<ull, NUM_BITBOARDS> pieceBitboards,
           std::array<ull, NUM_OCCUPANCIES> occupancyBitboards)
    : side(side), enPassant(enPassant), castle(castle), halfMoves(halfMoves),
      fullMoves(fullMoves), pieceBitboards(pieceBitboards),
      occupancyBitboards(occupancyBitboards) {
  init_all();
}

// iterates over all subsets of a occupancy mask to prepare it for sliding
// piece attack tables
ull Game::set_occupancy(int index, ull mask) {
  int count = 0;
  ull bitboard = 0;
  // iterate over set bits
  while (mask) {
    // get the index of that bit and pop it
    int lsbIndex = __builtin_ctzll(mask);
    pop_bit(mask, lsbIndex);

    // if the corresponding bit in index is set then set it in the new
    // bitboard
    if (index & (1 << count))
      bitboard |= (1ULL << lsbIndex);
    count++;
  }
  return bitboard;
}

ull Game::find_magic_number(int square, int relevantBits, int isBishop) {
  // Maximum amount of squares in a file or rank which could be blocker
  // squares (not needed)
  /*int maxRelevantSquares = 6;*/

  // Minimum bits in hashed numbers needed to avoid too frequent collisions
  int minHash = 6;
  // Need to consider combos of files and ranks. This is capped at 4096 = 2 ^
  // 12, so no need to compute it
  /*size_t attack_size = 1 << (2 * maxRelevantSquares);*/
  const int maxAttackSize = 4096;
  // extract top 8 bits from ull in order to check that the magicNumber is
  // "good"
  ull extract = 0xFF00000000000000;

  // initialize attack, usedAttack, and occupancy arrays
  ull attacks[maxAttackSize];
  ull usedAttacks[maxAttackSize];
  ull occupancies[maxAttackSize];

  // get the original attack mask (without any blockers)
  ull attack_mask =
      isBishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

  // total number of subsets of n different squares is 2^n
  int occupancy_indices = (1 << relevantBits);

  // Iterate through different subsets of the occupancy bits and instantiate
  // both the occupancy bits and the valid squares to move to with the current
  // mask
  for (int i = 0; i < occupancy_indices; i++) {
    occupancies[i] = set_occupancy(i, attack_mask);
    attacks[i] = isBishop ? generate_bishop_moves_fly(square, occupancies[i])
                          : generate_rook_moves_fly(square, occupancies[i]);
  }
  // magicNumber for hashing
  ull magicNumber;
  while (true) {
    // refer to this link for why we do & (we want number with less ones)
    // http://pradu.us/old/Nov27_2008/Buzz/research/magic/pieceBitboards.pdf
    magicNumber = generate_random_number() & generate_random_number() &
                  generate_random_number();
    bool currentMagicWorks = true;
    // at least 6 of 8 MSBits should be set
    if (__builtin_popcountll((magicNumber * attack_mask) & extract) < minHash)
      continue;

    // reset usedAttacks for this specific magic number
    memset(usedAttacks, 0, sizeof(usedAttacks));
    for (int i = 0; i < occupancy_indices; i++) {
      // multiplying by magicNumber does some random shifts and then shift
      // back to [0, 4096]
      int hash = (magicNumber * occupancies[i]) >> (64 - relevantBits);

      // if this hash is used by another board with a different bitmask of
      // possible moves, then magic number is no good
      if (!usedAttacks[hash]) {
        usedAttacks[hash] = attacks[i];
      } else if (usedAttacks[hash] != attacks[i]) {
        currentMagicWorks = false;
        break;
      }
    }
    if (currentMagicWorks)
      break;
  }
  return magicNumber;
}

// Fills the pre-calculated attack tables for leaper pieces
void Game::init_leaper_attacks() {
  for (int square = 0; square < NUM_SQ; square++) {
    for (auto side = 0; side < NUM_SIDES; side++) {
      pawnAttacks[square][side] = mask_pawn_attacks(square, side);
    }
    knightAttacks[square] = mask_knight_attacks(square);
    kingAttacks[square] = mask_king_attacks(square);
  }
}

void Game::init_magic_numbers() {
  for (int square = 0; square < NUM_SQ; square++) {
    rookMagics[square] = find_magic_number(square, rookRelevantBits[square], 0);
  }
  for (int square = 0; square < NUM_SQ; square++) {
    bishopMagics[square] =
        find_magic_number(square, bishopRelevantBits[square], 1);
  }
}

void Game::init_slider_attacks(int isBishop) {
  // initialize attack table
  for (int square = 0; square < NUM_SQ; square++) {
    if (isBishop)
      bishopMasks[square] = mask_bishop_attacks(square);
    else
      rookMasks[square] = mask_rook_attacks(square);
  }

  // iterate through all squares and
  // all subsets of the current mask and
  // then fill the attack tables
  for (int square = 0; square < NUM_SQ; square++) {
    // get the number of possible blocker squares
    const int &relevantBits =
        isBishop ? bishopRelevantBits[square] : rookRelevantBits[square];
    for (int index = 0; index < (1 << relevantBits); index++) {
      // get the actual occupancy mask based on index
      ull occupancy_mask = set_occupancy(index, isBishop ? bishopMasks[square]
                                                         : rookMasks[square]);
      // compute the hash of the occupancy mask to index into the attacks
      // array
      int hashedIndex =
          ((isBishop ? bishopMagics[square] : rookMagics[square]) *
           occupancy_mask) >>
          (NUM_SQ - relevantBits);

      // fill in the attack table with the hashed index
      if (isBishop)
        bishopAttacks[square][hashedIndex] =
            generate_bishop_moves_fly(square, occupancy_mask);
      else
        rookAttacks[square][hashedIndex] =
            generate_rook_moves_fly(square, occupancy_mask);
    }
  }
}

inline void Game::loop_attacks(int sourceSquare, ull attackBitboard,
                               int piece) {
  int otherSide = 1 - this->side;
  bool notPawn = (piece != p && piece != P);
  // iterate over the attacked square and ensure opponent's pieces are
  // present so capture is valid
  while (attackBitboard) {
    ull targetSquare = LSOneIndex(attackBitboard);
    //  check if there is an enemy piece or is it empty
    if (!get_bit(this->occupancyBitboards[both], targetSquare) && notPawn) {
      std::cout << pieceMap[piece] << " quiet move "
                << indexToSquare[sourceSquare] << "-"
                << indexToSquare[targetSquare] << "\n";
    } else if (get_bit(this->occupancyBitboards[otherSide], targetSquare)) {
      std::cout << pieceMap[piece] << " capture move "
                << indexToSquare[sourceSquare] << "-"
                << indexToSquare[targetSquare] << "\n";
    }
    pop_bit(attackBitboard, targetSquare);
  }
}

void Game::generate_moves() {
  // loop over all pieces

  std::string curColor = side == white ? "white" : "black";
  int otherSide = 1 - this->side;
  int sourceSquare, targetSquare;
  ull bitboardCopy, attackBitboard;
  int lowerPiece = (side ? p : P);
  int upperPiece = (side ? k : K);
  for (int piece = lowerPiece; piece <= upperPiece; piece++) {

    bitboardCopy = this->pieceBitboards[piece];
    if (side == white) {
      if (piece == P) {
        // generate moves in front
        while (bitboardCopy) {
          // get LSB of pawn bitboard
          sourceSquare = LSOneIndex(bitboardCopy);
          // check if can move up by 1
          targetSquare = sourceSquare - 8;
          if (!get_bit(this->occupancyBitboards[both], targetSquare)) {
            // check for promotion
            if (sourceSquare >= a7 && sourceSquare <= h7)
              std::cout << "White Pawn Promotion Move: "
                        << indexToSquare[sourceSquare] << "-"
                        << indexToSquare[targetSquare] << "\n";
            else
              std::cout << "White Pawn Quiet Move: "
                        << indexToSquare[sourceSquare] << "-"
                        << indexToSquare[targetSquare] << "\n";

            // check if pawn can move twice
            if (sourceSquare >= a2 && sourceSquare <= h2) {
              targetSquare -= 8;
              if (!get_bit(this->occupancyBitboards[both], targetSquare)) {
                std::cout << "White Pawn Double Move: "
                          << indexToSquare[sourceSquare] << "-"

                          << indexToSquare[targetSquare] << "\n";
              }
            }
          }

          attackBitboard = pawnAttacks[sourceSquare][side];

          loop_attacks(sourceSquare, attackBitboard, piece);
          pop_bit(bitboardCopy, sourceSquare);
        }
      } else if (piece == K) {
        sourceSquare = LSOneIndex(bitboardCopy);
        if (sourceSquare && !is_square_attacked(e1, otherSide)) {
          if ((wk & this->castle) &&
              !get_bit(this->occupancyBitboards[both], f1) &&
              !is_square_attacked(f1, black) &&
              !get_bit(this->occupancyBitboards[both], g1)) {

            std::cout << "White Kingside Castle"
                      << "\n";
          }
          if ((wq & this->castle) &&
              !get_bit(this->occupancyBitboards[both], d1) &&
              !is_square_attacked(d1, black) &&
              !get_bit(this->occupancyBitboards[both], c1) &&
              !get_bit(this->occupancyBitboards[both], b1)) {

            std::cout << "White Queenside Castle"
                      << "\n";
          }

          // get all squares that the king aattacks
          attackBitboard = kingAttacks[sourceSquare];
          loop_attacks(sourceSquare, attackBitboard, piece);
        }
      }
    } else if (side == black) {
      if (piece == p) {
        // generate moves in front
        while (bitboardCopy) {
          // get LSB of pawn bitboard
          sourceSquare = LSOneIndex(bitboardCopy);
          // check if can move up by 1
          targetSquare = sourceSquare + 8;
          if (!get_bit(this->occupancyBitboards[both], targetSquare)) {
            // check for promotion
            if (sourceSquare >= a2 && sourceSquare <= h2)
              std::cout << "Black Pawn Promotion Move: "
                        << indexToSquare[sourceSquare] << "-"
                        << indexToSquare[targetSquare] << "\n";
            else
              std::cout << "Black Pawn Quiet Move: "
                        << indexToSquare[sourceSquare] << "-"
                        << indexToSquare[targetSquare] << "\n";

            // check if pawn can move twice
            if (sourceSquare >= a7 && sourceSquare <= h7) {
              targetSquare += 8;
              if (!get_bit(this->occupancyBitboards[both], targetSquare)) {
                std::cout << "Black Pawn Double Move: "
                          << indexToSquare[sourceSquare] << "-"

                          << indexToSquare[targetSquare] << "\n";
              }
            }
          }

          attackBitboard = pawnAttacks[sourceSquare][side];

          loop_attacks(sourceSquare, attackBitboard, piece);
          pop_bit(bitboardCopy, sourceSquare);
        }
      } else if (piece == k) {
        sourceSquare = LSOneIndex(bitboardCopy);
        if (sourceSquare && !is_square_attacked(e8, otherSide)) {
          if ((bk & this->castle) &&
              !get_bit(this->occupancyBitboards[both], f8) &&
              !is_square_attacked(f8, white) &&
              !get_bit(this->occupancyBitboards[both], g8)) {

            std::cout << "Black King Side Castle"
                      << "\n";
          }
          if ((bq & this->castle) &&
              !get_bit(this->occupancyBitboards[both], d8) &&
              !is_square_attacked(d8, white) &&
              !get_bit(this->occupancyBitboards[both], c8) &&
              !get_bit(this->occupancyBitboards[both], b8)) {

            std::cout << "Black Queen Side Castle"
                      << "\n";
          }

          // get all squares that the king aattacks
          attackBitboard = kingAttacks[sourceSquare];
          loop_attacks(sourceSquare, attackBitboard, piece);
        }
      }
    }

    // can do all of the other pieces in one loop since no more special cases
    // need to make sure we don't accidentally add king or pawn moves again
    if (piece != (side ? p : P) && piece != (side ? k : K)) {
      while (bitboardCopy) {
        sourceSquare = LSOneIndex(bitboardCopy);

        // get the appropriate attackBitboard
        if (piece == (side ? n : N)) {
          attackBitboard = knightAttacks[sourceSquare];
        } else if (piece == (side ? b : B)) {
          // and with the negation of you side's piece since you can't capture
          // ur own pieces
          attackBitboard =
              get_bishop_attack(sourceSquare, this->occupancyBitboards[both],
                                this->bishopMasks, this->bishopAttacks,
                                this->bishopMagics) &
              ~this->occupancyBitboards[side];
        } else if (piece == (side ? r : R)) {
          attackBitboard =
              get_rook_attack(sourceSquare, this->occupancyBitboards[both],
                              this->rookMasks, this->rookAttacks,
                              this->rookMagics) &
              ~this->occupancyBitboards[side];
        } else if (piece == (side ? q : Q)) {
          attackBitboard =
              get_queen_attack(sourceSquare, this->occupancyBitboards[both],
                               this->rookMasks, this->rookAttacks,
                               this->rookMagics, this->bishopMasks,
                               this->bishopAttacks, this->bishopMagics) &
              ~this->occupancyBitboards[side];
        }
        loop_attacks(sourceSquare, attackBitboard, piece);
        pop_bit(bitboardCopy, sourceSquare);
      }
    }
  }
}
/**
 * Checks to see if a square is attacked by side. It works by generating moves
 * by different pieces of the opposite side's color, and then checking if the
 * current side has any pieces there. For example, if the square is e3 and side
 * is white, then the function will simulate a black pawn on e3, knight on e3,
 * etc. and then get the squares that the piece is attacking (i.e. g2, c2, d1,
 * etc. for the black knight). it will then check if there is a white piece on
 * those squares, and return true if so. For slider pieces like rooks, it also
 * takes into account the full occupancyBitboard
 */

void Game::init_all() {
  init_magic_numbers();
  init_leaper_attacks();
  init_slider_attacks(rook);
  init_slider_attacks(bishop);
}
