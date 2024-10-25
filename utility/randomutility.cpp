#include "macros.h"
ull initial_state = INITIAL_RANDOM_STATE;
ull generate_random_number() {
  ull cp = initial_state;
  cp ^= cp << 13;
  cp ^= cp >> 7;
  cp ^= cp << 17;
  initial_state = cp;
  return cp;
}
