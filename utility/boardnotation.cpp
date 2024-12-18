/*
 * Prints squares in algebraic notation (a1, a2, ...) in
 * several different formats
 */
#include "../bitboard/bitboard.h"
#include "macros.h"
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
  for (int row = 0; row < BD; row++) {
    pspace(1);
    std::cout << 8 - row;
    pspace(1);
    for (int col = 0; col < BD; col++) {
      int ix = BD * row + col;
      std::cout << ((get_bit(bitboard, ix)) != 0) << " ";
    }
    std::cout << "\n";
  }

  // extra space for the row number + a little padding
  pspace(3);
  for (int col = 0; col < BD; col++) {
    std::cout << static_cast<char>('a' + col) << ' ';
  }
  std::cout << "\n\n";

  std::cout << "Bitboard value is: " << bitboard << "\n";
  std::cout << "\n";
}

void print_board(Game &game) {
  // loop through every square of the bitboard and print its value
  std::cout << "\n";
  for (int row = 0; row < BD; row++) {
    pspace(1);
    std::cout << 8 - row;
    pspace(1);
    for (int col = 0; col < BD; col++) {
      int square = BD * row + col;
      int curPiece = -1;
      // iterate over all piece bitboards and check if that piece is on that
      // sqaure
      for (int piece = P; piece <= k; piece++)
        if (get_bit(game.pieceBitboards[piece], square))
          curPiece = piece;

      /*std::cout << (curPiece == -1 ? '.' : asciiPieces[curPiece]) << " ";*/
      std::cout << (curPiece == -1 ? "." : unicodePieces[curPiece]) << " ";
    }
    std::cout << "\n";
  }

  // extra space for the row number + a little padding
  pspace(3);
  for (int col = 0; col < BD; col++) {
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
            << (game.enPassant == NO_SQ ? "None"
                                        : indexToSquare[game.enPassant])
            << "\n";
  std::cout << "\n";
}

void print_move(int move, int spacing) {
  int src = decode_src(move);
  int dst = decode_dst(move);
  int piece = decode_piece(move);
  int promo = decode_promotion(move);
  int castle = decode_castle(move);
  int capture = decode_capture(move);
  int doublePush = decode_double_push(move);
  int enPassant = decode_en_passant(move);

  std::cout << std::left << std::setw(spacing) << indexToSquare[src]
            << std::setw(spacing) << indexToSquare[dst] << std::setw(spacing)
            << asciiPieces[piece] << std::setw(spacing)
            << ((promo == 0) ? std::string("None")
                             : std::string(1, asciiPieces[promo]))
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
