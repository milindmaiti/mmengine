#include "../movegen/bishopmove.h"
#include "../movegen/kingmove.h"
#include "../movegen/knightmove.h"
#include "../movegen/pawnmove.h"
#include "../movegen/rookmove.h"
#include "../utility/boardnotation.h"
#include "../utility/genspecialboards.h"
#include "../utility/macros.h"
#include "../utility/randomutility.h"
#include <functional>

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
ull pieceBitboard[NUM_BITBOARDS];
ull occupancypieceBitboard[NUM_OCCUPANCIES];
// side to move
int side;
// en passant capture square
int enPassant = NO_SQ;
// castling rights
int castle;

/* pre-calculated pawn attack tables */
ull pawnAttacks[NUM_SQ][NUM_SIDES];
/* pre-calculated knight attack tables */
ull knightAttacks[NUM_SQ];
/* pre-calculated king attack tables */
ull kingAttacks[NUM_SQ];

// rook attack masks (considering no blockers)
ull rookMasks[NUM_SQ];
// bishop attack masks (considering no blockers)
ull bishopMasks[NUM_SQ];

// rook attack masks (indexed by square and blocker piece masks)
ull rookAttacks[NUM_SQ][1 << MAX_BISHOP_BLOCK];
// bishop attack masks (indexed by square and blocker piece masks)
ull bishopAttacks[NUM_SQ][1 << MAX_ROOK_BLOCK];

/* pre-calculated rook magic numbers */
ull rookMagics[NUM_SQ] = {
    0x5800840802010000ULL, 0x2040204000100000ULL, 0x8008502081448004ULL,
    0x200200208c090000ULL, 0x2040142002001000ULL, 0xc840082042011800ULL,
    0x9040092002880800ULL, 0x2012180049010000ULL, 0x2200104021830000ULL,
    0x824040410a240000ULL, 0x1304001044108060ULL, 0xc04002010428007ULL,
    0x14000400501008ULL,   0x200a002a0420000ULL,  0xa0802207004a8800ULL,
    0x22488a10050000ULL,   0x24c0800000ULL,       0x40040820300ULL,
    0x4010200005ULL,       0x4000009006404000ULL, 0x84040008145002ULL,
    0x80002d12000ULL,      0x4080010000ULL,       0x860802400510400ULL,
    0x40300080800000ULL,   0x420180030040000ULL,  0x83300200100ULL,
    0x4000380034100005ULL, 0xc5400158400ULL,      0x4090081200420000ULL,
    0x5024020800ULL,       0x80088620014000ULL,   0x4500042020120000ULL,
    0x10000a4220c00000ULL, 0x6000021040080082ULL, 0x800404000ULL,
    0x4800000460802000ULL, 0x40000482001400ULL,   0x1234000100808000ULL,
    0xc000a01000ULL,       0x280024000828000ULL,  0x4014130040a40000ULL,
    0x8a9002c8000ULL,      0x140001110010001ULL,  0x8100002040040011ULL,
    0x640004204008000ULL,  0x88040302084c8000ULL, 0x80480908240a00ULL,
    0x8002102040841400ULL, 0x4000100c8320400ULL,  0x200020211a00010ULL,
    0x1400001a00509010ULL, 0x100000800440080ULL,  0x1000000402001200ULL,
    0x88004910020080ULL,   0x802002700049040ULL,  0x801620400104024ULL,
    0x1100008104004024ULL, 0x10020000c41ULL,      0x800014202c0004ULL,
    0x800001004082002ULL,  0x120001016810a48ULL,  0x1100106208009104ULL,
    0x2000000100b40042ULL};
/* PRE-CALCULATED BISHOP MAGIC NUMBERS */
ull bishopMagics[NUM_SQ] = {
    0x248081102450402ULL,  0x3810220821001118ULL, 0x802281044c100a0ULL,
    0x8020424000208ULL,    0x4062021082100440ULL, 0x202404440200000cULL,
    0x40c041e028040ULL,    0x2800808c01010110ULL, 0x9c08810a4008009ULL,
    0x9081000c09004ULL,    0x40842444000100ULL,   0x1091840000000ULL,
    0x8000011002008000ULL, 0x140089014000800ULL,  0x20201210000ULL,
    0x804020104228000ULL,  0x3015002022012020ULL, 0x8301102811280200ULL,
    0x50200c1080282000ULL, 0x884000a046009210ULL, 0x200009222008090ULL,
    0x140210802080109ULL,  0x8ae40000ULL,         0x1080005484042280ULL,
    0xa0806042040404a0ULL, 0x441100024628000ULL,  0x840220400924000ULL,
    0x404002044002000ULL,  0x4800001014404020ULL, 0x200800440840400dULL,
    0x40000e0210512405ULL, 0x20022200a8211104ULL, 0x6006202300a4ULL,
    0x80010400b080a0ULL,   0x109040480001ULL,     0x1040000810040042ULL,
    0x8881101008190000ULL, 0x8086828020ULL,       0x400004640001104cULL,
    0x400404a082002000ULL, 0x80710008042ULL,      0xa80020825544000ULL,
    0x45000024016010c0ULL, 0x1123000010440860ULL, 0x81001000b09000ULL,
    0x2000310010a0000ULL,  0x10000844a2900104ULL, 0x1000421060410001ULL,
    0x2220024802280000ULL, 0x14124200201ULL,      0x8030120896010880ULL,
    0x14201240000ULL,      0xe000014044090100ULL, 0x4100044a908c0000ULL,
    0x2101002104020ULL,    0x800800d1020101ULL,   0x200208022a2a00ULL,
    0x1480080901140ULL,    0x44010004420200ULL,   0x5000800c01028200ULL,
    0x1000100240250ULL,    0x802000802001a480ULL, 0x20000208c80900ULL,
    0x800c0404820018ULL};

// iterates over all subsets of a occupancy mask to prepare it for sliding piece
// attack tables
ull set_occupancy(int index, ull mask) {
  int count = 0;
  ull bitboard = 0;
  // iterate over set bits
  while (mask) {
    // get the index of that bit and pop it
    int lsbIndex = __builtin_ctzll(mask);
    pop_bit(mask, lsbIndex);

    // if the corresponding bit in index is set then set it in the new bitboard
    if (index & (1 << count))
      bitboard |= (1ULL << lsbIndex);
    count++;
  }
  return bitboard;
}

ull find_magic_number(int square, int relevantBits, int isBishop) {
  // Maximum amount of squares in a file or rank which could be blocker squares
  // (not needed)
  /*int maxRelevantSquares = 6;*/

  // Minimum bits in hashed numbers needed to avoid too frequent collisions
  int minHash = 6;
  // Need to consider combos of files and ranks. This is capped at 4096 = 2 ^
  // 12, so no need to compute it
  /*size_t attack_size = 1 << (2 * maxRelevantSquares);*/
  int maxAttackSize = 4096;
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
    // http://pradu.us/old/Nov27_2008/Buzz/research/magic/pieceBitboard.pdf
    magicNumber = generate_random_number() & generate_random_number() &
                  generate_random_number();
    bool currentMagicWorks = true;
    // at least 6 of 8 MSBits should be set
    if (__builtin_popcountll((magicNumber * attack_mask) & extract) < minHash)
      continue;

    // reset usedAttacks for this specific magic number
    memset(usedAttacks, 0, sizeof(usedAttacks));
    for (int i = 0; i < occupancy_indices; i++) {
      // multiplying by magicNumber does some random shifts and then shift back
      // to [0, 4096]
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
void init_leaper_attacks() {
  for (int square = 0; square < NUM_SQ; square++) {
    for (auto side = 0; side < NUM_SIDES; side++) {
      pawnAttacks[square][side] = mask_pawn_attacks(square, side);
    }
    knightAttacks[square] = mask_knight_attacks(square);
    kingAttacks[square] = mask_king_attacks(square);
  }
}

void init_magic_numbers() {
  for (int square = 0; square < NUM_SQ; square++) {
    rookMagics[square] = find_magic_number(square, rookRelevantBits[square], 0);
  }
  for (int square = 0; square < NUM_SQ; square++) {
    bishopMagics[square] =
        find_magic_number(square, bishopRelevantBits[square], 1);
  }
}

void init_slider_attacks(int isBishop) {
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
      // compute the hash of the occupancy mask to index into the attacks array
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
static inline ull get_bishop_attack(int square, ull occupancyMask) {
  // take only the blocker bits from the mask
  occupancyMask &= bishopMasks[square];
  return bishopAttacks[square][(bishopMagics[square] * occupancyMask) >>
                               (NUM_SQ - bishopRelevantBits[square])];
}

// returns a mask of squares a rook can move to given other piece placement
static inline ull get_rook_attack(int square, ull occupancyMask) {
  // take only the blocker bits from the mask
  occupancyMask &= rookMasks[square];
  return rookAttacks[square][(rookMagics[square] * occupancyMask) >>
                             (NUM_SQ - rookRelevantBits[square])];
}

void init_all() {
  /*init_magic_numbers();*/
  init_leaper_attacks();
  init_slider_attacks(rook);
  init_slider_attacks(bishop);
}

int main() {
  init_all();
  ull bitboard = 0ULL;
  int square = d4;
  set_bit(bitboard, e4);
  set_bit(bitboard, d7);
  set_bit(bitboard, d6);
  set_bit(bitboard, c4);
  set_bit(bitboard, f6);
  print_bitboard(get_bishop_attack(square, bitboard));
  print_bitboard(get_rook_attack(square, bitboard));
  for (int square = a7; square <= h7; square++)
    set_bit(pieceBitboard[P], square);
  for (int square = a2; square <= h2; square++)
    set_bit(pieceBitboard[p], square);

  set_bit(pieceBitboard[Q], d8);
  set_bit(pieceBitboard[K], e8);
  set_bit(pieceBitboard[B], f8);
  set_bit(pieceBitboard[B], c8);
  set_bit(pieceBitboard[N], b8);
  set_bit(pieceBitboard[N], g8);
  set_bit(pieceBitboard[R], a8);
  set_bit(pieceBitboard[R], h8);

  set_bit(pieceBitboard[q], d1);
  set_bit(pieceBitboard[k], e1);
  set_bit(pieceBitboard[b], f1);
  set_bit(pieceBitboard[b], c1);
  set_bit(pieceBitboard[n], b1);
  set_bit(pieceBitboard[n], g1);
  set_bit(pieceBitboard[r], a1);
  set_bit(pieceBitboard[r], h1);
  print_bitboard(pieceBitboard[P]);
  printBoard(pieceBitboard, white, 15, e4);
}
