/*
 * Prints squares in algebraic notation (a1, a2, ...) in
 * several different formats
 */
#include "bitboard/bitboard.h"
#include "utility/macros.h"
#include <iomanip>
#include <iostream>

// prints a8, b8, ... in order without quotes
void print_no_quotes() {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      char file = static_cast<char>('a' + j);
      std::cout << file << i + 1 << ", ";
    }
    std::cout << "\n";
  }
}

// prints a8, b8, ... in order with quotes
void print_quotes() {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      char file = static_cast<char>('a' + j);
      std::cout << "\"" << file << i + 1 << "\"" << ", ";
    }
    std::cout << "\n";
  }
}

void print_bitboard(ull bitboard) {

  // spaces on the left side

  // loop through every square of the bitboard and print its value
  std::cout << "\n";
  for (ull row = 0; row < Constants::BD; row++) {
    PrintUtil::pspace(1);
    std::cout << 8 - row;
    PrintUtil::pspace(1);
    for (ull col = 0; col < Constants::BD; col++) {
      int ix = Constants::BD * row + col;
      std::cout << ((BitUtil::get_bit(bitboard, ix)) != 0) << " ";
    }
    std::cout << "\n";
  }

  // extra space for the row number + a little padding
  PrintUtil::pspace(3);
  for (ull col = 0; col < Constants::BD; col++) {
    std::cout << static_cast<char>('a' + col) << ' ';
  }
  std::cout << "\n\n";

  std::cout << "Bitboard value is: " << bitboard << "\n";
  std::cout << "\n";
}

void print_board(Game &game) {
  // loop through every square of the bitboard and print its value
  std::cout << "\n";
  for (ull row = 0; row < Constants::BD; row++) {
    PrintUtil::pspace(1);
    std::cout << 8 - row;
    PrintUtil::pspace(1);
    for (ull col = 0; col < Constants::BD; col++) {
      int square = Constants::BD * row + col;
      int curPiece = -1;
      // iterate over all piece bitboards and check if that piece is on that
      // sqaure
      for (int piece = Notation::P; piece <= Notation::k; piece++)
        if (BitUtil::get_bit(game.pieceBitboards[piece], square))
          curPiece = piece;

      /*std::cout << (curPiece == -1 ? '.' : asciiPieces[curPiece]) << " ";*/
      std::cout << (curPiece == -1 ? "." : ArrayUtil::unicodePieces[curPiece])
                << " ";
    }
    std::cout << "\n";
  }

  // extra space for the row number + a little padding
  PrintUtil::pspace(3);
  for (ull col = 0; col < Constants::BD; col++) {
    std::cout << static_cast<char>('a' + col) << ' ';
  }
  std::cout << "\n\n";

  std::cout << "Side to move: " << (game.side ? "black" : "white") << "\n";
  std::cout << "Castling Rights: ";
  std::cout << ((game.castle & 1) ? "wk " : "");
  std::cout << ((game.castle & 2) ? "wq " : "");
  std::cout << ((game.castle & 4) ? "bk " : "");
  std::cout << ((game.castle & 8) ? "bq " : "");
  std::cout << "\n";

  std::cout << "En Passant Square is: "
            << (game.enPassant == Notation::NO_SQ
                    ? "None"
                    : ArrayUtil::indexToSquare[game.enPassant])
            << "\n";
  std::cout << "\n";
}

void print_move(int move, int spacing) {
  int src = BitUtil::decode_src(move);
  int dst = BitUtil::decode_dst(move);
  int piece = BitUtil::decode_piece(move);
  int promo = BitUtil::decode_promotion(move);
  int castle = BitUtil::decode_castle(move);
  int capture = BitUtil::decode_capture(move);
  int doublePush = BitUtil::decode_double_push(move);
  int enPassant = BitUtil::decode_en_passant(move);

  std::cout << std::left << std::setw(spacing) << ArrayUtil::indexToSquare[src]
            << std::setw(spacing) << ArrayUtil::indexToSquare[dst]
            << std::setw(spacing) << ArrayUtil::asciiPieces[piece]
            << std::setw(spacing)
            << ((promo == 0) ? std::string("None")
                             : std::string(1, ArrayUtil::asciiPieces[promo]))
            << std::setw(spacing) << (castle ? "Yes" : "No")

            << std::setw(spacing) << (capture ? "Yes" : "No")
            << std::setw(spacing) << (doublePush ? "Yes" : "No")
            << std::setw(spacing) << (enPassant ? "Yes" : "No") << "\n";
}

void print_moves(std::vector<int> &moveList) {
  std::cout << "\t\t ---------- MOVE LIST ----------\n\n";
  int spacing = 12;
  std::vector<std::string> categories = {"Source",     "Dest",     "Piece",
                                         "PromoPiece", "Castle",   "Capture",
                                         "DoublePush", "EnPassant"};
  for (std::string &cat : categories) {
    std::cout << std::left << std::setw(spacing) << cat;
  }
  std::cout << "\n\n";

  for (int move : moveList)
    print_move(move, spacing);

  std::cout << "\n";
  std::cout << "Number of Moves: " << moveList.size() << "\n";
}
