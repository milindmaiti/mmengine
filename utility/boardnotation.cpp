/*
 * Prints squares in algebraic notation (a1, a2, ...) in
 * several different formats
 */
#include "macros.h"
#include <iostream>

// prints a8, b8, ... in order without quotes
void printNoQuotes() {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      char file = static_cast<char>('a' + j);
      std::cout << file << i + 1 << ", ";
    }
    std::cout << "\n";
  }
}

// prints a8, b8, ... in order with quotes
void printQuotes() {
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
#ifdef TEST_BOARDNOTATION
int main() { printQuotes(); }
#endif
