#pragma once
#include <array>
#include <iostream>
#include <string>
// Common sizes / expressions macros
#define ull unsigned long long
#define BD 8
#define NUM_SQ 64
#define NUM_SIDES 2
#define ASCII_SZ 256
#define NUM_PIECES 12

// Printing macros
// clang-format off
#define pspace(num) for(int _ = 0; _ < num; _++){std::cout << " ";}
// clang-format on

// Random Number Generator Seed
#define INITIAL_RANDOM_STATE 5654047531167510087ULL

// Board lookup macros
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))
#define toggle_bit(bitboard, square) ((bitboard) ^= (1ULL << (square)))

#define copy_current_board()                                                   \
  std::array<ull, NUM_BITBOARDS> copyPieceBitboards = game.pieceBitboards;     \
  std::array<ull, NUM_OCCUPANCIES> copyOccupancyBitboards =                    \
      game.occupancyBitboards;                                                 \
  int copySide = game.side;                                                    \
  int copyEnPassant = game.enPassant;                                          \
  int copyCastle = game.castle;                                                \
  int copyHalfMoves = game.halfMoves;                                          \
  int copyFullMoves = game.fullMoves;

#define pop_current_copy()                                                     \
  game.pieceBitboards = copyPieceBitboards;                                    \
  game.occupancyBitboards = copyOccupancyBitboards;                            \
  game.side = copySide;                                                        \
  game.enPassant = copyEnPassant;                                              \
  game.castle = copyCastle;                                                    \
  game.halfMoves = copyHalfMoves;                                              \
  game.fullMoves = copyFullMoves;
// move encoding/decoding macros
#define encode_move(src, dst, piece, promotion, castle, capture, doublePush,   \
                    enPassant)                                                 \
  ((src) + (1 << 6) * (dst) + (1 << 12) * (piece) + (1 << 16) * (promotion) +  \
   (1 << 20) * (castle) + (1 << 21) * (capture) + (1 << 22) * (doublePush) +   \
   (1 << 23) * (enPassant))

// move decoding macros
#define decode_src(move) ((move) & 0x3F)
#define decode_dst(move) (((move) & 0xFC0) >> 6)
#define decode_piece(move) (((move) & 0xF000) >> 12)
#define decode_promotion(move) (((move) & 0xF0000) >> 16)
#define decode_castle(move) (((move) & 0x100000) >> 20)
#define decode_capture(move) (((move) & 0x200000) >> 21)
#define decode_double_push(move) (((move) & 0x400000) >> 22)
#define decode_en_passant(move) (((move) & 0x800000) >> 23)

// the value for no promotion when encoding/decoding
#define NO_PROMOTION 15
// number of bitboards to represent full chess board
#define NUM_BITBOARDS 12
// 3 occupancies boards for black, white, and full
#define NUM_OCCUPANCIES 3

// max blocker macros
#define MAX_BISHOP_BLOCK 9
#define MAX_ROOK_BLOCK 12

// bit manipulation macros
#define LSOne(S) ((S) & -(S))
#define LSOneIndex(S) __builtin_ctzll(LSOne(S))
// Special Board Definitions
#define NO_H_FILE 9187201950435737471ULL
#define NO_A_FILE 18374403900871474942ULL
#define NO_AB_FILE 18229723555195321596ULL
#define NO_HG_FILE 4557430888798830399ULL
// algebraic notation to index lookup
// clang-format off
extern const std::array<std::string, NUM_SQ> indexToSquare;
extern const std::array<int,NUM_SQ> bishopRelevantBits;
extern const std::array<int,NUM_SQ> rookRelevantBits;
extern const std::string asciiPieces;
extern const std::array<std::string, NUM_PIECES> unicodePieces;
extern const std::array<int, ASCII_SZ> charPieces;
extern const std::array<ull, NUM_SQ> rookMagics;
extern const std::array<ull, NUM_SQ> bishopMagics;
extern const std::string emptyPosition;
extern const std::string startPosition;
extern const std::string trickyPosition;
extern const std::string killerPosition;
extern const std::string cmkPosition;
extern const std::array<std::string, NUM_PIECES> pieceMap;
extern const std::array<int, NUM_SQ> castleMasks;

enum sliderPiece { rook, bishop };
enum castling { wk = 1, wq = 2, bk = 4, bq = 8};
enum pieces {P, N, B, R, Q, K, p, n, b, r, q, k};
enum algebraicNotation{
  a8, b8, c8, d8, e8, f8, g8, h8, 
a7, b7, c7, d7, e7, f7, g7, h7, 
a6, b6, c6, d6, e6, f6, g6, h6, 
a5, b5, c5, d5, e5, f5, g5, h5, 
a4, b4, c4, d4, e4, f4, g4, h4, 
a3, b3, c3, d3, e3, f3, g3, h3, 
a2, b2, c2, d2, e2, f2, g2, h2, 
a1, b1, c1, d1, e1, f1, g1, h1, NO_SQ 
};
enum colors{
  white, black, both
};

// clang-format on
