#pragma once
#include <array>
#include <string>
// Common sizes / expressions macros
#define ull unsigned long long

namespace Constants {
  constexpr ull BD = 8;
  constexpr ull NUM_SQ = 64;
  constexpr ull NUM_SIDES = 2;
  constexpr ull ASCII_SZ = 256;
  constexpr ull NUM_PIECES = 12;

  // Random Number Generator Seed
  constexpr ull RANDOM_STATE = 5654047531167510087ULL;

  // the value for no promotion when encoding/decoding
  constexpr ull NO_PROMOTION = 15;
  // number of bitboards to represent full chess board
  constexpr ull NUM_BITBOARDS = 12;
  // 3 occupancies boards for black, white, and full
  constexpr ull NUM_OCCUPANCIES = 3;

  // Special Board Definitions
  constexpr ull NO_H_FILE = 9187201950435737471ULL;
  constexpr ull NO_A_FILE = 18374403900871474942ULL;
  constexpr ull NO_AB_FILE = 18229723555195321596ULL;
  constexpr ull NO_HG_FILE = 4557430888798830399ULL;

  // max blocker macros
  constexpr ull MAX_BISHOP_BLOCK = 9;
  constexpr ull MAX_ROOK_BLOCK = 12;
} // namespace Constants

namespace PrintUtil {
  void pspace(int num);
}; // namespace PrintUtil

namespace BitUtil {
  ull get_bit(ull bitboard, ull square);
  ull set_bit(ull bitboard, ull square);
  ull pop_bit(ull bitboard, ull square);
  ull toggle_bit(ull bitboard, ull square);

  // move encoding/decoding functions
  ull encode_move(ull src, ull dst, ull piece, ull promotion, ull castle,
                  ull capture, ull doublePush, ull enPassant);
  ull decode_src(ull move);
  ull decode_dst(ull move);
  ull decode_piece(ull move);
  ull decode_promotion(ull move);
  ull decode_castle(ull move);
  ull decode_capture(ull move);
  ull decode_double_push(ull move);
  ull decode_en_passant(ull move);

  // bit manipulation helpers
  template <typename T> inline T LSOne(T S) { return (S & -S); }
  template <typename T> inline int LSOneIndex(T S) {
    return __builtin_ctzll(LSOne(S));
  }
} // namespace BitUtil

namespace ArrayUtil {
  // algebraic notation to index lookup
  // clang-format off
  extern const std::array<std::string, Constants::NUM_SQ> indexToSquare;
  extern const std::array<int,Constants::NUM_SQ> bishopRelevantBits;
  extern const std::array<int,Constants::NUM_SQ> rookRelevantBits;
  extern const std::array<int, Constants::NUM_PIECES> materialScores;
  extern const std::string asciiPieces;
  extern const std::array<std::string, Constants::NUM_PIECES> unicodePieces;
  extern const std::array<int, Constants::ASCII_SZ> charPieces;
  extern const std::array<ull, Constants::NUM_SQ> rookMagics;
  extern const std::array<ull, Constants::NUM_SQ> bishopMagics;
  extern const std::string emptyPosition;
  extern const std::string startPosition;
  extern const std::string trickyPosition;
  extern const std::string killerPosition;
  extern const std::string cmkPosition;
  extern const std::array<std::string, Constants::NUM_PIECES> pieceMap;
  extern const std::array<int, Constants::NUM_SQ> castleMasks;
  extern const std::array<int, Constants::NUM_SQ> squareMirror;
}

namespace Notation{
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
}

// clang-format on
