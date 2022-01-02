
#include "splash/pokemons.hpp"

#include "splash/evolve_table.hpp"
#include "splash/operator.hpp"
#include "splash/random.hpp"

Pokemons* Pokemons::instance = nullptr;

Pokemons::Pokemons() { this->RandomizePokemonMap(); }

Pokemons* Pokemons::GetInstance() {
    if (instance == nullptr) {
        instance = new Pokemons;
    }
    return instance;
}

void Pokemons::RandomizePokemonMap() {
    int32_t seed = PlayerWork__get_uniqueID(0);

    // create map
    for (uint16_t x = 0; x < POKEMON_COUNT; x++) {
        this->map[x] = x + 1;
    }

    Random rand = Random(seed);
    rand.ShuffleList(&this->map[0], POKEMON_COUNT);
};

uint16_t Pokemons::GetRandom(uint16_t monsno) {
    // return value from shuffled map
    if (monsno > POKEMON_COUNT || monsno < 1) {
        throw "Invalid pokemon";
    }
    return this->map[monsno - 1];
};

uint16_t Pokemons::GetRandom(uint16_t monsno, uint8_t level) {
    // Fix impossible pokemon level
    uint16_t mons = this->GetRandom(monsno);
    evolve ev = {.prevmons = 0, .prevlvl = 0};
    do {
        ev = evolve_table[mons - 1];
        if (ev.prevlvl > level) {
            mons = ev.prevmons;
        }
    } while (ev.prevmons != 0 && ev.prevlvl > level);
    return mons;
};

void Pokemons::RandomizeMonsLvArray(MonsLv_array* mons) {
    il2cpp_array_size_t len = mons->max_length;
    for (uint32_t idx = 0; idx < len; idx++) {
        MonsLv_Fields* obj = &(mons->m_Items[idx].fields);

        if (obj->monsNo != 0) {
            // obj->maxlv += 1;
            // obj->minlv = obj->maxlv;
            obj->monsNo = this->GetRandom(obj->monsNo, obj->maxlv);
        }
    }
};

XLSXContent_FieldEncountTable_Sheettable_o* Pokemons::RandomizeGameManagerEncountData(int32_t zoneid) {
    XLSXContent_FieldEncountTable_Sheettable_o* real = GameManager__GetFieldEncountData(zoneid, 0);
    if (real == 0) {
        return 0;
    }
    this->RandomizeMonsLvArray(real->fields.ground_mons);
    this->RandomizeMonsLvArray(real->fields.tairyo);
    this->RandomizeMonsLvArray(real->fields.day);
    this->RandomizeMonsLvArray(real->fields.night);
    this->RandomizeMonsLvArray(real->fields.swayGrass);

    this->RandomizeMonsLvArray(real->fields.gbaRuby);
    this->RandomizeMonsLvArray(real->fields.gbaSapp);
    this->RandomizeMonsLvArray(real->fields.gbaEme);
    this->RandomizeMonsLvArray(real->fields.gbaFire);
    this->RandomizeMonsLvArray(real->fields.gbaLeaf);

    this->RandomizeMonsLvArray(real->fields.water_mons);
    this->RandomizeMonsLvArray(real->fields.boro_mons);
    this->RandomizeMonsLvArray(real->fields.ii_mons);
    this->RandomizeMonsLvArray(real->fields.sugoi_mons);
    return real;
};
