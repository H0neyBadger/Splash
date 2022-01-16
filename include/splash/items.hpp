#ifndef ITEMS_H
#define ITEMS_H

#include <stdint.h>

#include "splash/game.h"
#include "splash/random.hpp"
#include "splash/tables.hpp"

// last randomized item TM92
#define ITEM_COUNT 419

class Items {
   private:
    Items();
    static Items* instance;
    uint16_t map[ITEM_COUNT];
    void RandomizeItemMap();
    bool IsValid(uint16_t itemno);

   public:
    // singleton
    Items(const Items&) = delete;
    void operator=(const Items&) = delete;
    static Items* GetInstance();

    uint16_t GetRandom(uint16_t itemno);
};

#endif  // ndef ITEMS_H
