#include "macros.h"

// Initialize with engine RNG seed
static ull initial_state = Constants::RANDOM_STATE;
// generate 64 bit random numbers using the XOR shift method
ull generate_random_number() {
  ull cp = initial_state;
  cp ^= cp << 13;
  cp ^= cp >> 7;
  cp ^= cp << 17;
  initial_state = cp;
  return cp;
}
