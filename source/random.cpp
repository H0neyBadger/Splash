
#include "splash/random.hpp"

void
Random::ShuffleList(uint16_t* lst, uint32_t len)
{
  int32_t j, seed = this->seed;
  uint16_t swap;
  uint64_t r;

  if (seed == 0) {
    throw "Invalid seed";
  }

  for (uint32_t i = len; i > 1; i--) {
    // naive attempt to create randomness from seed
    // this is not a real random
    r = i * (uint64_t)seed;
    seed ^= r & 0xffffffff;
    seed ^= r >> 32 & 0xffffffff;

    // https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
    j = seed % i; // random in range
    swap = lst[j];
    lst[j] = lst[i];
    lst[i] = swap;
  }
}
