#ifndef POKEMONS_H
#define POKEMONS_H

#include <stdint.h>

#include "splash/game.h"
#include "splash/random.hpp"
#include "splash/tables.hpp"

#define POKEMON_COUNT 493

class Pokemons {
   private:
    Pokemons();
    static Pokemons* instance;
    uint16_t map[POKEMON_COUNT];
    void RandomizePokemonMap();
    void RandomizeMonsLvArray(MonsLv_array* mons);
    void RandomizeTranerPoke(trainer_poke* obj);

   public:
    // singleton
    Pokemons(const Pokemons&) = delete;
    void operator=(const Pokemons&) = delete;
    static Pokemons* GetInstance();

    uint16_t GetRandom(uint16_t monsno);
    uint16_t GetRandom(uint16_t monsno, uint8_t level);

    XLSXContent_FieldEncountTable_Sheettable_o* RandomizeGameManagerEncountData(int32_t zoneid);
    XLSXContent_TrainerTable_SheetTrainerPoke_o* RandomizeTrainerPoke(int32_t trainer_id);
};

#endif  // ndef POKEMONS_H
