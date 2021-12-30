#ifndef POKEMONS_H
#define POKEMONS_H

#include "splash/game.h"
#include "splash/random.hpp"

#include <cstdint>

#define POKEMON_COUNT 493

class Pokemons
{
private:
  uint16_t map[POKEMON_COUNT];
  void RandomizeMonsLvArray(MonsLv_array* mons);

public:
  Pokemons();
  void RandomizePokemonMap();
  uint16_t GetRandom(uint16_t in);
  XLSXContent_FieldEncountTable_Sheettable_o* RandomizeGameManagerEncountData(
    int32_t zoneid);
};

#endif // ndef POKEMONS_H
