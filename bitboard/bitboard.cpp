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
#include <iostream>
#include <cstring>
#include <vector>

#define copy_board()                                                           \
  std::array<ull, NUM_BITBOARDS> copyPieceBitboards = this->pieceBitboards;    \
  std::array<ull, NUM_OCCUPANCIES> copyOccupancyBitboards =                    \
      this->occupancyBitboards;                                                \
  int copySide = this->side;                                                   \
  int copyEnPassant = this->enPassant;                                         \
  int copyCastle = this->castle;                                               \
  int copyHalfMoves = this->halfMoves;                                         \
  int copyFullMoves = this->fullMoves;

#define pop_copy()                                                             \
  this->pieceBitboards = copyPieceBitboards;                                   \
  this->occupancyBitboards = copyOccupancyBitboards;                           \
  this->side = copySide;                                                       \
  this->enPassant = copyEnPassant;                                             \
  this->castle = copyCastle;                                                   \
  this->halfMoves = copyHalfMoves;                                             \
  this->fullMoves = copyFullMoves;

Game::Game(std::array<ull, NUM_SQ> rookMagics,
           std::array<ull, NUM_SQ> bishopMagics, int side, int enPassant,
           int castle, int halfMoves, int fullMoves,
           std::array<ull, NUM_BITBOARDS> pieceBitboards,
           std::array<ull, NUM_OCCUPANCIES> occupancyBitboards)
    : pieceBitboards(pieceBitboards), occupancyBitboards(occupancyBitboards),
      side(side), enPassant(enPassant), castle(castle), halfMoves(halfMoves),
      fullMoves(fullMoves), rookMagics(rookMagics), bishopMagics(bishopMagics) {
  init_leaper_attacks();
  init_slider_attacks(rook);
  init_slider_attacks(bishop);
}

Game::Game(int side, int enPassant, int castle, int halfMoves, int fullMoves,
           std::array<ull, NUM_BITBOARDS> pieceBitboards,
           std::array<ull, NUM_OCCUPANCIES> occupancyBitboards)
    : pieceBitboards(pieceBitboards), occupancyBitboards(occupancyBitboards),
      side(side), enPassant(enPassant), castle(castle), halfMoves(halfMoves),
      fullMoves(fullMoves) {
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

inline void Game::loop_attacks(int sourceSquare, ull attackBitboard, int piece,
                               std::vector<int> &moveList) {
  int otherSide = 1 - this->side;
  bool isPawn = (piece == p || piece == P);
  // iterate over the attacked square and ensure opponent's pieces are
  // present so capture is valid
  while (attackBitboard) {
    int targetSquare = LSOneIndex(attackBitboard);
    //  check if there is an enemy piece or is it empty
    if (!get_bit(this->occupancyBitboards[both], targetSquare)) {
      /*std::cout << pieceMap[piece] << " quiet move "*/
      /*          << indexToSquare[sourceSquare] << "-"*/
      /*          << indexToSquare[targetSquare] << "\n";*/

      if (isPawn) {
        if (targetSquare == this->enPassant)
          moveList.push_back(
              encode_move(sourceSquare, targetSquare, piece, 0, 0, 1, 0, 1));
      } else
        moveList.push_back(
            encode_move(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));

    } else if (get_bit(this->occupancyBitboards[otherSide], targetSquare)) {
      /*std::cout << pieceMap[piece] << " capture move "*/
      /*          << indexToSquare[sourceSquare] << "-"*/
      /*          << indexToSquare[targetSquare] << "\n";*/

      // if we are capturing and promoting from either white or black side
      if ((piece == P && (sourceSquare >= a7 && sourceSquare <= h7))) {
        for (int promoPiece = N; promoPiece <= Q; promoPiece++) {

          moveList.push_back(encode_move(sourceSquare, targetSquare, piece,
                                         promoPiece, 0, 1, 0, 0));
        }
      } else if ((piece == p && (sourceSquare >= a2 && sourceSquare <= h2))) {
        for (int promoPiece = n; promoPiece <= q; promoPiece++) {
          moveList.push_back(encode_move(sourceSquare, targetSquare, piece,
                                         promoPiece, 0, 1, 0, 0));
        }
      } else {
        moveList.push_back(
            encode_move(sourceSquare, targetSquare, piece, 0, 0, 1, 0, 0));
      }
    }
    pop_bit(attackBitboard, targetSquare);
  }
}

std::vector<int> Game::generate_moves() {
  // loop over all pieces
  std::vector<int> moveList;

  std::string curColor = side == white ? "white" : "black";
  int otherSide = 1 - this->side;
  int sourceSquare, targetSquare;
  ull bitboardCopy, attackBitboard = 0;
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
            if (sourceSquare >= a7 && sourceSquare <= h7) {
              for (int promoPiece = N; promoPiece <= Q; promoPiece++) {

                moveList.push_back(encode_move(sourceSquare, targetSquare,
                                               piece, promoPiece, 0, 0, 0, 0));
                /*std::cout << "White Pawn Promotion Move: "*/
                /*          << indexToSquare[sourceSquare] << "-"*/
                /*          << indexToSquare[targetSquare] << "="*/
                /*          << asciiPieces[promoPiece] << "\n";*/
              }
            } else {

              moveList.push_back(encode_move(sourceSquare, targetSquare, piece,
                                             0, 0, 0, 0, 0));
              /*std::cout << "White Pawn Quiet Move: "*/
              /*          << indexToSquare[sourceSquare] << "-"*/
              /*          << indexToSquare[targetSquare] << "\n";*/
            }

            // check if pawn can move twice
            if (sourceSquare >= a2 && sourceSquare <= h2) {
              targetSquare -= 8;
              if (!get_bit(this->occupancyBitboards[both], targetSquare)) {
                moveList.push_back(encode_move(sourceSquare, targetSquare,
                                               piece, 0, 0, 0, 1, 0));
                /*std::cout << "White Pawn Double Move: "*/
                /*          << indexToSquare[sourceSquare] << "-"*/
                /**/
                /*          << indexToSquare[targetSquare] << "\n";*/
              }
            }
          }

          attackBitboard = pawnAttacks[sourceSquare][side];
          loop_attacks(sourceSquare, attackBitboard, piece, moveList);
          pop_bit(bitboardCopy, sourceSquare);
        }
      } else if (piece == K) {
        sourceSquare = LSOneIndex(bitboardCopy);
        if (!is_square_attacked(e1, otherSide)) {
          if ((wk & this->castle) &&
              !get_bit(this->occupancyBitboards[both], f1) &&
              !is_square_attacked(f1, black) &&
              !get_bit(this->occupancyBitboards[both], g1)) {

            moveList.push_back(encode_move(e1, g1, piece, 0, 1, 0, 0, 0));
            /*std::cout << "White Kingside Castle"*/
            /*          << "\n";*/
          }
          if ((wq & this->castle) &&
              !get_bit(this->occupancyBitboards[both], d1) &&
              !is_square_attacked(d1, black) &&
              !get_bit(this->occupancyBitboards[both], c1) &&
              !get_bit(this->occupancyBitboards[both], b1)) {

            moveList.push_back(encode_move(e1, c1, piece, 0, 1, 0, 0, 0));
            /*std::cout << "White Queenside Castle"*/
            /*          << "\n";*/
          }
        }
        // get all squares that the king aattacks
        attackBitboard = kingAttacks[sourceSquare];
        loop_attacks(sourceSquare, attackBitboard, piece, moveList);
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
            if (sourceSquare >= a2 && sourceSquare <= h2) {

              for (int promoPiece = n; promoPiece < k; promoPiece++) {

                moveList.push_back(encode_move(sourceSquare, targetSquare,
                                               piece, promoPiece, 0, 0, 0, 0));
                /*std::cout << "Black Pawn Promotion Move: " <<
                 * indexToSquare[sourceSquare] << "-"*/
                /*          << indexToSquare[targetSquare] << "\n";*/
              }
            } else {

              /*std::cout << "Black Pawn Quiet Move: "*/
              /*          << indexToSquare[sourceSquare] << "-"*/
              /*          << indexToSquare[targetSquare] << "\n";*/
              moveList.push_back(encode_move(sourceSquare, targetSquare, piece,
                                             0, 0, 0, 0, 0));
            }

            // check if pawn can move twice
            if (sourceSquare >= a7 && sourceSquare <= h7) {
              targetSquare += 8;
              if (!get_bit(this->occupancyBitboards[both], targetSquare)) {
                /*std::cout << "Black Pawn Double Move: "*/
                /*          << indexToSquare[sourceSquare] << "-"*/
                /**/
                /*          << indexToSquare[targetSquare] << "\n";*/
                moveList.push_back(encode_move(sourceSquare, targetSquare,
                                               piece, 0, 0, 0, 1, 0));
              }
            }
          }

          attackBitboard = pawnAttacks[sourceSquare][side];

          loop_attacks(sourceSquare, attackBitboard, piece, moveList);
          pop_bit(bitboardCopy, sourceSquare);
        }
      } else if (piece == k) {
        sourceSquare = LSOneIndex(bitboardCopy);
        if (!is_square_attacked(e8, otherSide)) {
          if ((bk & this->castle) &&
              !get_bit(this->occupancyBitboards[both], f8) &&
              !is_square_attacked(f8, white) &&
              !get_bit(this->occupancyBitboards[both], g8)) {

            moveList.push_back(encode_move(e8, g8, piece, 0, 1, 0, 0, 0));
            /*std::cout << "Black King Side Castle"*/
            /*          << "\n";*/
          }
          if ((bq & this->castle) &&
              !get_bit(this->occupancyBitboards[both], d8) &&
              !is_square_attacked(d8, white) &&
              !get_bit(this->occupancyBitboards[both], c8) &&
              !get_bit(this->occupancyBitboards[both], b8)) {

            /*std::cout << "Black Queen Side Castle"*/
            /*          << "\n";*/
            moveList.push_back(encode_move(e8, c8, piece, 0, 1, 0, 0, 0));
          }
        }
        // get all squares that the king attacks
        attackBitboard = kingAttacks[sourceSquare];
        loop_attacks(sourceSquare, attackBitboard, piece, moveList);
      }
    }

    // can do all of the other pieces in one loop since no more special cases
    // need to make sure we don't accidentally add king or pawn moves again
    if (piece != (side == black ? p : P) && piece != (side == black ? k : K)) {
      while (bitboardCopy) {
        sourceSquare = LSOneIndex(bitboardCopy);

        // get the appropriate attackBitboard
        if (piece == (side ? n : N)) {
          attackBitboard = knightAttacks[sourceSquare];
        } else if (piece == (side ? b : B)) {
          // and with the negation of your side's piece since you can't capture
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
        loop_attacks(sourceSquare, attackBitboard, piece, moveList);
        pop_bit(bitboardCopy, sourceSquare);
      }
    }
  }
  return moveList;
}

int Game::makeMove(int move, bool onlyCapture) {

  if (onlyCapture && !decode_capture(move))
    return 0;
  copy_board();
  int src = decode_src(move);
  int dst = decode_dst(move);
  int piece = decode_piece(move);
  int promo = decode_promotion(move);
  int castle = decode_castle(move);
  int capture = decode_capture(move);
  int doublePush = decode_double_push(move);
  int enPassant = decode_en_passant(move);
  pop_bit(this->pieceBitboards[piece], src);
  set_bit(this->pieceBitboards[piece], dst);
  this->castle &= castleMasks[src];
  this->castle &= castleMasks[dst];
  // if a piece is capture pop it out of the corresponding bitboard
  if (capture) {
    int lowerPiece, higherPiece;
    if (side == white) {
      lowerPiece = p, higherPiece = q;
    } else {
      lowerPiece = P, higherPiece = Q;
    }
    for (int capturedPiece = lowerPiece; capturedPiece <= higherPiece;
         capturedPiece++) {
      if (get_bit(this->pieceBitboards[capturedPiece], dst)) {
        pop_bit(this->pieceBitboards[capturedPiece], dst);
        break;
      }
    }
  }
  // if there's a promotion replace the dst square with the promoted piece
  if (((piece == P) && (dst >= a8 && dst <= h8)) ||
      ((piece == p) && (dst >= a1 && dst <= h1))) {
    pop_bit(this->pieceBitboards[piece], dst);
    set_bit(this->pieceBitboards[promo], dst);
  }

  // remove the opposite side pawn if enpassant move has been played
  if (enPassant) {
    pop_bit(this->pieceBitboards[(side == white ? p : P)],
            this->enPassant + (side == white ? 8 : -8));
  }

  this->enPassant = NO_SQ;
  // set the enpassant square appropriately depending on how the pawn moves
  if (doublePush) {
    this->enPassant = dst + ((this->side == white) ? 8 : -8);
  }
  // move the rook to complete castling
  if (castle) {
    if (dst == g1) {
      pop_bit(this->pieceBitboards[R], h1);
      set_bit(this->pieceBitboards[R], f1);
    } else if (dst == c1) {
      pop_bit(this->pieceBitboards[R], a1);
      set_bit(this->pieceBitboards[R], d1);
    } else if (dst == g8) {
      pop_bit(this->pieceBitboards[r], h8);
      set_bit(this->pieceBitboards[r], f8);
    } else {
      pop_bit(this->pieceBitboards[r], a8);
      set_bit(this->pieceBitboards[r], d8);
    }
  }

  // copy all white pieces to the full white bitboard and same for black

  this->occupancyBitboards[white] = this->occupancyBitboards[black] =
      this->occupancyBitboards[both] = 0ULL;
  for (int i = P; i <= K; i++)
    this->occupancyBitboards[white] |= this->pieceBitboards[i];
  for (int i = p; i <= k; i++)
    this->occupancyBitboards[black] |= this->pieceBitboards[i];

  // copy all white + black pieces to full bitboard
  this->occupancyBitboards[both] =
      (this->occupancyBitboards[white] | this->occupancyBitboards[black]);

  if (is_square_attacked(
          LSOneIndex(this->pieceBitboards[(side == white) ? K : k]), !side)) {
    pop_copy();
    return 0;
  } else {
    side = !side;
    return 1;
  }
}
void Game::init_all() {
  init_magic_numbers();
  init_leaper_attacks();
  init_slider_attacks(rook);
  init_slider_attacks(bishop);
}

#undef copy_board
#undef pop_copy
