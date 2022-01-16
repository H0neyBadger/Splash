
#include "splash/random.hpp"

void Random::ShuffleList(uint16_t* lst, uint32_t len) {
    uint16_t swap;
    uint32_t j, seed = this->seed;
    uint64_t r;

    if (seed == 0) {
        throw "Invalid seed";
    }

    for (uint32_t i = len - 1; i > 0; i--) {
        // naive attempt to create randomness from seed
        // this is not a real random
        r = i * (uint64_t)seed;
        if (r != seed) {
            seed ^= r & 0xffffffff;
        }
        if (r >> 32 != seed) {
            seed ^= (r >> 32) & 0xffffffff;
        }

        // https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
        j = seed % i;  // FIXME modulo is biased
        swap = lst[j];
        lst[j] = lst[i];
        lst[i] = swap;
    }
}
