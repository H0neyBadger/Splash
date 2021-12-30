#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>

class Random
{
private:
  uint32_t seed = 0; // trainerId
public:
  Random(uint32_t seed)
    : seed{ seed } {};
  void ShuffleList(uint16_t* lst, uint32_t len);
};

#endif // ndef RANDOM_H
