#include "bitboard.h"
#include "../movegen/bishopmove.h"
#include "../movegen/kingmove.h"
#include "../movegen/knightmove.h"
#include "../movegen/pawnmove.h"
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
  init_all();
}

Game::Game(int side, int enPassant, int castle, int halfMoves, int fullMoves,
           std::array<ull, NUM_BITBOARDS> pieceBitboards,
           std::array<ull, NUM_OCCUPANCIES> occupancyBitboards)
    : side(side), enPassant(enPassant), castle(castle), halfMoves(halfMoves),
      fullMoves(fullMoves) {
  pieceBitboards.fill(0);
  occupancyBitboards.fill(0);
  init_all();
  init_magic_numbers();
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

// returns a mask of squares a bishop can move to given other piece placement
inline ull Game::get_bishop_attack(int square, ull occupancyMask) {
  // take only the blocker bits from the mask
  occupancyMask &= bishopMasks[square];
  return bishopAttacks[square][(bishopMagics[square] * occupancyMask) >>
                               (NUM_SQ - bishopRelevantBits[square])];
}

// returns a mask of squares a rook can move to given other piece placement
inline ull Game::get_rook_attack(int square, ull occupancyMask) {
  // take only the blocker bits from the mask
  occupancyMask &= rookMasks[square];
  return rookAttacks[square][(rookMagics[square] * occupancyMask) >>
                             (NUM_SQ - rookRelevantBits[square])];
}

void Game::init_all() {
  /*init_magic_numbers();*/
  init_leaper_attacks();
  init_slider_attacks(rook);
  init_slider_attacks(bishop);
}
