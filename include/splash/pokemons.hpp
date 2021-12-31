#ifndef POKEMONS_H
#define POKEMONS_H

#include <stdint.h>

#include "splash/game.h"
#include "splash/random.hpp"

#define POKEMON_COUNT 493

class Pokemons {
   private:
    Pokemons();
    static Pokemons* instance;
    uint16_t map[POKEMON_COUNT];
    void RandomizePokemonMap();
    void RandomizeMonsLvArray(MonsLv_array* mons);

   public:
    // singleton
    Pokemons(const Pokemons&) = delete;
    void operator=(const Pokemons&) = delete;
    static Pokemons* GetInstance();

    uint16_t GetRandom(uint16_t in);
    XLSXContent_FieldEncountTable_Sheettable_o* RandomizeGameManagerEncountData(int32_t zoneid);
};

#endif  // ndef POKEMONS_H
