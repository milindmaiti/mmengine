#include "bitboard.h"
#include "movegen/bishopmove.h"
#include "movegen/kingmove.h"
#include "movegen/knightmove.h"
#include "movegen/pawnmove.h"
#include "movegen/queenmove.h"
#include "movegen/rookmove.h"
#include "utility/macros.h"
#include "utility/randomutility.h"
#include <array>
#include <vector>

Game::Game(const std::array<ull, Constants::NUM_SQ> &rookMagics,
           const std::array<ull, Constants::NUM_SQ> &bishopMagics, int side,
           int enPassant, int castle, int halfMoves, int fullMoves,
           std::array<ull, Constants::NUM_BITBOARDS> pieceBitboards,
           std::array<ull, Constants::NUM_OCCUPANCIES> occupancyBitboards)
    : rookMagics(rookMagics), bishopMagics(bishopMagics), side(side),
      enPassant(enPassant), castle(castle), halfMoves(halfMoves),
      fullMoves(fullMoves), pieceBitboards(pieceBitboards),
      occupancyBitboards(occupancyBitboards) {
  init_leaper_attacks();
  init_slider_attacks(Notation::rook);
  init_slider_attacks(Notation::bishop);
}

Game::Game(int side, int enPassant, int castle, int halfMoves, int fullMoves,
           std::array<ull, Constants::NUM_BITBOARDS> pieceBitboards,
           std::array<ull, Constants::NUM_OCCUPANCIES> occupancyBitboards)
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
    BitUtil::pop_bit(mask, lsbIndex);

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
  for (ull square = 0; square < Constants::NUM_SQ; square++) {
    for (ull side = 0; side < Constants::NUM_SIDES; side++) {
      pawnAttacks[square][side] = mask_pawn_attacks(square, side);
    }
    knightAttacks[square] = mask_knight_attacks(square);
    kingAttacks[square] = mask_king_attacks(square);
  }
}

void Game::init_magic_numbers() {
  for (ull square = 0; square < Constants::NUM_SQ; square++) {
    rookMagics[square] =
        find_magic_number(square, ArrayUtil::rookRelevantBits[square], 0);
  }
  for (ull square = 0; square < Constants::NUM_SQ; square++) {
    bishopMagics[square] =
        find_magic_number(square, ArrayUtil::bishopRelevantBits[square], 1);
  }
}

void Game::init_slider_attacks(int isBishop) {
  // initialize attack table
  for (ull square = 0; square < Constants::NUM_SQ; square++) {
    if (isBishop)
      bishopMasks[square] = mask_bishop_attacks(square);
    else
      rookMasks[square] = mask_rook_attacks(square);
  }

  // iterate through all squares and
  // all subsets of the current mask and
  // then fill the attack tables
  for (ull square = 0; square < Constants::NUM_SQ; square++) {
    // get the number of possible blocker squares
    const int &relevantBits = isBishop ? ArrayUtil::bishopRelevantBits[square]
                                       : ArrayUtil::rookRelevantBits[square];
    for (int index = 0; index < (1 << relevantBits); index++) {
      // get the actual occupancy mask based on index
      ull occupancy_mask = set_occupancy(index, isBishop ? bishopMasks[square]
                                                         : rookMasks[square]);
      // compute the hash of the occupancy mask to index into the attacks
      // array
      int hashedIndex =
          ((isBishop ? bishopMagics[square] : rookMagics[square]) *
           occupancy_mask) >>
          (Constants::NUM_SQ - relevantBits);

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
  bool isPawn = (piece == Notation::p || piece == Notation::P);
  // iterate over the attacked square and ensure opponent's pieces are
  // present so capture is valid
  while (attackBitboard) {
    int targetSquare = BitUtil::LSOneIndex(attackBitboard);
    //  check if there is an enemy piece or is it empty
    if (!BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                          targetSquare)) {
      /*std::cout << pieceMap[piece] << " quiet move "*/
      /*          << indexToSquare[sourceSquare] << "-"*/
      /*          << indexToSquare[targetSquare] << "\n";*/

      if (isPawn) {
        if (targetSquare == this->enPassant)
          moveList.push_back(BitUtil::encode_move(sourceSquare, targetSquare,
                                                  piece, 0, 0, 0, 0, 1));
      } else
        moveList.push_back(BitUtil::encode_move(sourceSquare, targetSquare,
                                                piece, 0, 0, 0, 0, 0));

    } else if (BitUtil::get_bit(this->occupancyBitboards[otherSide],
                                targetSquare)) {
      /*std::cout << pieceMap[piece] << " capture move "*/
      /*          << indexToSquare[sourceSquare] << "-"*/
      /*          << indexToSquare[targetSquare] << "\n";*/

      // if we are capturing and promoting from either white or black side
      if ((piece == Notation::P &&
           (sourceSquare >= Notation::a7 && sourceSquare <= Notation::h7))) {
        for (int promoPiece = Notation::N; promoPiece <= Notation::Q;
             promoPiece++) {

          moveList.push_back(BitUtil::encode_move(
              sourceSquare, targetSquare, piece, promoPiece, 0, 1, 0, 0));
        }
      } else if ((piece == Notation::p && (sourceSquare >= Notation::a2 &&
                                           sourceSquare <= Notation::h2))) {
        for (int promoPiece = Notation::n; promoPiece <= Notation::q;
             promoPiece++) {
          moveList.push_back(BitUtil::encode_move(
              sourceSquare, targetSquare, piece, promoPiece, 0, 1, 0, 0));
        }
      } else {
        moveList.push_back(BitUtil::encode_move(sourceSquare, targetSquare,
                                                piece, 0, 0, 1, 0, 0));
      }
    }
    BitUtil::pop_bit(attackBitboard, targetSquare);
  }
}

std::vector<int> Game::generate_moves() {
  // loop over all pieces
  std::vector<int> moveList;

  std::string curColor = side == Notation::white ? "white" : "black";
  int otherSide = 1 - this->side;
  int sourceSquare, targetSquare;
  ull bitboardCopy, attackBitboard;
  int lowerPiece = (side ? Notation::p : Notation::P);
  int upperPiece = (side ? Notation::k : Notation::K);
  for (int piece = lowerPiece; piece <= upperPiece; piece++) {
    bitboardCopy = this->pieceBitboards[piece];
    if (side == Notation::white) {
      if (piece == Notation::P) {
        // generate moves in front
        while (bitboardCopy) {
          // get LSB of pawn bitboard
          sourceSquare = BitUtil::LSOneIndex(bitboardCopy);
          // check if can move up by 1
          targetSquare = sourceSquare - 8;
          if (!BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                targetSquare)) {
            // check for promotion
            if (sourceSquare >= Notation::a7 && sourceSquare <= Notation::h7) {
              for (int promoPiece = Notation::N; promoPiece <= Notation::Q;
                   promoPiece++) {

                moveList.push_back(BitUtil::encode_move(
                    sourceSquare, targetSquare, piece, promoPiece, 0, 0, 0, 0));
                /*std::cout << "White Pawn Promotion Move: "*/
                /*          << indexToSquare[sourceSquare] << "-"*/
                /*          << indexToSquare[targetSquare] << "="*/
                /*          << asciiPieces[promoPiece] << "\n";*/
              }
            } else {

              moveList.push_back(BitUtil::encode_move(
                  sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));
              /*std::cout << "White Pawn Quiet Move: "*/
              /*          << indexToSquare[sourceSquare] << "-"*/
              /*          << indexToSquare[targetSquare] << "\n";*/
            }

            // check if pawn can move twice
            if (sourceSquare >= Notation::a2 && sourceSquare <= Notation::h2) {
              targetSquare -= 8;
              if (!BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                    targetSquare)) {
                moveList.push_back(BitUtil::encode_move(
                    sourceSquare, targetSquare, piece, 0, 0, 0, 1, 0));
                /*std::cout << "White Pawn Double Move: "*/
                /*          << indexToSquare[sourceSquare] << "-"*/
                /**/
                /*          << indexToSquare[targetSquare] << "\n";*/
              }
            }
          }

          attackBitboard = pawnAttacks[sourceSquare][side];
          loop_attacks(sourceSquare, attackBitboard, piece, moveList);
          BitUtil::pop_bit(bitboardCopy, sourceSquare);
        }
      } else if (piece == Notation::K) {
        sourceSquare = BitUtil::LSOneIndex(bitboardCopy);
        if (!is_square_attacked(Notation::e1, otherSide)) {
          if ((Notation::wk & this->castle) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::f1) &&
              !is_square_attacked(Notation::f1, Notation::black) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::g1)) {

            moveList.push_back(BitUtil::encode_move(Notation::e1, Notation::g1,
                                                    piece, 0, 1, 0, 0, 0));
            /*std::cout << "White Kingside Castle"*/
            /*          << "\n";*/
          }
          if ((Notation::wq & this->castle) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::d1) &&
              !is_square_attacked(Notation::d1, Notation::black) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::c1) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::b1)) {

            moveList.push_back(BitUtil::encode_move(Notation::e1, Notation::c1,
                                                    piece, 0, 1, 0, 0, 0));
            /*std::cout << "White Queenside Castle"*/
            /*          << "\n";*/
          }
        }
        // get all squares that the king aattacks
        attackBitboard = kingAttacks[sourceSquare];
        loop_attacks(sourceSquare, attackBitboard, piece, moveList);
      }
    } else if (side == Notation::black) {
      if (piece == Notation::p) {
        // generate moves in front
        while (bitboardCopy) {
          // get LSB of pawn bitboard
          sourceSquare = BitUtil::LSOneIndex(bitboardCopy);
          // check if can move up by 1
          targetSquare = sourceSquare + 8;
          if (!BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                targetSquare)) {
            // check for promotion
            if (sourceSquare >= Notation::a2 && sourceSquare <= Notation::h2) {

              for (int promoPiece = Notation::n; promoPiece < Notation::k;
                   promoPiece++) {

                moveList.push_back(BitUtil::encode_move(
                    sourceSquare, targetSquare, piece, promoPiece, 0, 0, 0, 0));
                /*std::cout << "Black Pawn Promotion Move: " <<
                 * indexToSquare[sourceSquare] << "-"*/
                /*          << indexToSquare[targetSquare] << "\n";*/
              }
            } else {

              /*std::cout << "Black Pawn Quiet Move: "*/
              /*          << indexToSquare[sourceSquare] << "-"*/
              /*          << indexToSquare[targetSquare] << "\n";*/
              moveList.push_back(BitUtil::encode_move(
                  sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0));
            }

            // check if pawn can move twice
            if (sourceSquare >= Notation::a7 && sourceSquare <= Notation::h7) {
              targetSquare += 8;
              if (!BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                    targetSquare)) {
                /*std::cout << "Black Pawn Double Move: "*/
                /*          << indexToSquare[sourceSquare] << "-"*/
                /**/
                /*          << indexToSquare[targetSquare] << "\n";*/
                moveList.push_back(BitUtil::encode_move(
                    sourceSquare, targetSquare, piece, 0, 0, 0, 1, 0));
              }
            }
          }

          attackBitboard = pawnAttacks[sourceSquare][side];

          loop_attacks(sourceSquare, attackBitboard, piece, moveList);
          BitUtil::pop_bit(bitboardCopy, sourceSquare);
        }
      } else if (piece == Notation::k) {
        sourceSquare = BitUtil::LSOneIndex(bitboardCopy);
        if (!is_square_attacked(Notation::e8, otherSide)) {
          if ((Notation::bk & this->castle) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::f8) &&
              !is_square_attacked(Notation::f8, Notation::white) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::g8)) {

            moveList.push_back(BitUtil::encode_move(Notation::e8, Notation::g8,
                                                    piece, 0, 1, 0, 0, 0));
            /*std::cout << "Black King Side Castle"*/
            /*          << "\n";*/
          }
          if ((Notation::bq & this->castle) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::d8) &&
              !is_square_attacked(Notation::d8, Notation::white) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::c8) &&
              !BitUtil::get_bit(this->occupancyBitboards[Notation::both],
                                Notation::b8)) {

            /*std::cout << "Black Queen Side Castle"*/
            /*          << "\n";*/
            moveList.push_back(BitUtil::encode_move(Notation::e8, Notation::c8,
                                                    piece, 0, 1, 0, 0, 0));
          }
        }
        // get all squares that the king attacks
        attackBitboard = kingAttacks[sourceSquare];
        loop_attacks(sourceSquare, attackBitboard, piece, moveList);
      }
    }

    // can do all of the other pieces in one loop since no more special cases
    // need to make sure we don't accidentally add king or pawn moves again
    if (piece != (side == Notation::black ? Notation::p : Notation::P) &&
        piece != (side == Notation::black ? Notation::k : Notation::K)) {
      while (bitboardCopy) {
        sourceSquare = BitUtil::LSOneIndex(bitboardCopy);

        // get the appropriate attackBitboard
        if (piece == (side ? Notation::n : Notation::N)) {
          attackBitboard = knightAttacks[sourceSquare];
        } else if (piece == (side ? Notation::b : Notation::B)) {
          // and with the negation of your side's piece since you can't capture
          // ur own pieces
          attackBitboard =
              get_bishop_attack(
                  sourceSquare, this->occupancyBitboards[Notation::both],
                  this->bishopMasks, this->bishopAttacks, this->bishopMagics) &
              ~this->occupancyBitboards[side];
        } else if (piece == (side ? Notation::r : Notation::R)) {
          attackBitboard =
              get_rook_attack(
                  sourceSquare, this->occupancyBitboards[Notation::both],
                  this->rookMasks, this->rookAttacks, this->rookMagics) &
              ~this->occupancyBitboards[side];
        } else if (piece == (side ? Notation::q : Notation::Q)) {
          attackBitboard =
              get_queen_attack(
                  sourceSquare, this->occupancyBitboards[Notation::both],
                  this->rookMasks, this->rookAttacks, this->rookMagics,
                  this->bishopMasks, this->bishopAttacks, this->bishopMagics) &
              ~this->occupancyBitboards[side];
        }
        loop_attacks(sourceSquare, attackBitboard, piece, moveList);
        BitUtil::pop_bit(bitboardCopy, sourceSquare);
      }
    }
  }
  return moveList;
}

int Game::makeMove(int move, bool onlyCapture) {

  if (onlyCapture && !BitUtil::decode_capture(move))
    return 0;

  BoardState state = this->saveState();
  int src = BitUtil::decode_src(move);
  int dst = BitUtil::decode_dst(move);
  int piece = BitUtil::decode_piece(move);
  int promo = BitUtil::decode_promotion(move);
  int castle = BitUtil::decode_castle(move);
  int capture = BitUtil::decode_capture(move);
  int doublePush = BitUtil::decode_double_push(move);
  int enPassant = BitUtil::decode_en_passant(move);
  BitUtil::pop_bit(this->pieceBitboards[piece], src);
  BitUtil::set_bit(this->pieceBitboards[piece], dst);
  this->castle &= ArrayUtil::castleMasks[src];
  this->castle &= ArrayUtil::castleMasks[dst];
  // if a piece is capture pop it out of the corresponding bitboard
  if (capture) {
    int lowerPiece, higherPiece;
    if (side == Notation::white) {
      lowerPiece = Notation::p, higherPiece = Notation::q;
    } else {
      lowerPiece = Notation::P, higherPiece = Notation::Q;
    }
    for (int capturedPiece = lowerPiece; capturedPiece <= higherPiece;
         capturedPiece++) {
      if (BitUtil::get_bit(this->pieceBitboards[capturedPiece], dst)) {
        BitUtil::pop_bit(this->pieceBitboards[capturedPiece], dst);
        break;
      }
    }
  }
  // if there's a promotion replace the dst square with the promoted piece
  if (((piece == Notation::P) &&
       (dst >= Notation::a8 && dst <= Notation::h8)) ||
      ((piece == Notation::p) &&
       (dst >= Notation::a1 && dst <= Notation::h1))) {
    BitUtil::pop_bit(this->pieceBitboards[piece], dst);
    BitUtil::set_bit(this->pieceBitboards[promo], dst);
  }

  // remove the opposite side pawn if enpassant move has been played
  if (enPassant) {
    BitUtil::pop_bit(this->pieceBitboards[(
                         side == Notation::white ? Notation::p : Notation::P)],
                     this->enPassant + (side == Notation::white ? 8 : -8));
  }

  this->enPassant = Notation::NO_SQ;
  // set the enpassant square appropriately depending on how the pawn moves
  if (doublePush) {
    this->enPassant = dst + ((this->side == Notation::white) ? 8 : -8);
  }
  // move the rook to complete castling
  if (castle) {
    if (dst == Notation::g1) {
      BitUtil::pop_bit(this->pieceBitboards[Notation::R], Notation::h1);
      BitUtil::set_bit(this->pieceBitboards[Notation::R], Notation::f1);
    } else if (dst == Notation::c1) {
      BitUtil::pop_bit(this->pieceBitboards[Notation::R], Notation::a1);
      BitUtil::set_bit(this->pieceBitboards[Notation::R], Notation::d1);
    } else if (dst == Notation::g8) {
      BitUtil::pop_bit(this->pieceBitboards[Notation::r], Notation::h8);
      BitUtil::set_bit(this->pieceBitboards[Notation::r], Notation::f8);
    } else {
      BitUtil::pop_bit(this->pieceBitboards[Notation::r], Notation::a8);
      BitUtil::set_bit(this->pieceBitboards[Notation::r], Notation::d8);
    }
  }

  // copy all white pieces to the full white bitboard and same for black

  this->occupancyBitboards[Notation::white] =
      this->occupancyBitboards[Notation::black] =
          this->occupancyBitboards[Notation::both] = 0ULL;
  for (int i = Notation::P; i <= Notation::K; i++)
    this->occupancyBitboards[Notation::white] |= this->pieceBitboards[i];
  for (int i = Notation::p; i <= Notation::k; i++)
    this->occupancyBitboards[Notation::black] |= this->pieceBitboards[i];

  // copy all white + black pieces to full bitboard
  this->occupancyBitboards[Notation::both] =
      (this->occupancyBitboards[Notation::white] |
       this->occupancyBitboards[Notation::black]);

  if (is_square_attacked(
          BitUtil::LSOneIndex(
              this->pieceBitboards[(side == Notation::white) ? Notation::K
                                                             : Notation::k]),
          !side)) {
    this->restoreState(state);
    return 0;
  } else {
    side = !side;
    return 1;
  }
}
void Game::init_all() {
  init_magic_numbers();
  init_leaper_attacks();
  init_slider_attacks(Notation::rook);
  init_slider_attacks(Notation::bishop);
}

BoardState Game::saveState() const {
  return BoardState{pieceBitboards, occupancyBitboards, side,     enPassant,
                    castle,         halfMoves,          fullMoves};
}

void Game::restoreState(const BoardState &state) {
  pieceBitboards = state.pieceBitboards;
  occupancyBitboards = state.occupancyBitboards;
  side = state.side;
  enPassant = state.enPassant;
  castle = state.castle;
  halfMoves = state.halfMoves;
  fullMoves = state.fullMoves;
}
