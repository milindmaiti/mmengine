#include "boardnotation.h"
#include "macros.h"
#include <iostream>

void no_a_file() {
  ull bitboard = 0ULL;
  for (int i = 0; i < BD; i++) {
    for (int j = 0; j < BD; j++) {
      int ix = BD * i + j;
      if (j != 0)
        bitboard |= (1ULL << ix);
    }
  }
  print_bitboard(bitboard);
}
void no_h_file() {
  ull bitboard = 0ULL;
  for (int i = 0; i < BD; i++) {
    for (int j = 0; j < BD; j++) {
      int ix = BD * i + j;
      if (j != BD - 1)
        bitboard |= (1ULL << ix);
    }
  }
  print_bitboard(bitboard);
}
void no_hg_file() {
  ull bitboard = 0ULL;
  for (int i = 0; i < BD; i++) {
    for (int j = 0; j < BD; j++) {
      int ix = BD * i + j;
      if (j < 6)
        bitboard |= (1ULL << ix);
    }
  }
  print_bitboard(bitboard);
}
void no_ab_file() {
  ull bitboard = 0ULL;
  for (int i = 0; i < BD; i++) {
    for (int j = 0; j < BD; j++) {
      int ix = BD * i + j;
      if (j > 1)
        bitboard |= (1ULL << ix);
    }
  }
  print_bitboard(bitboard);
}

#ifdef TEST_GENSPECIALBOARDS
int main() {
  no_a_file();
  no_h_file();
  no_ab_file();
  no_hg_file();
}
#endif
