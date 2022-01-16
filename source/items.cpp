
#include "splash/items.hpp"

#include "splash/operator.hpp"
#include "splash/random.hpp"

Items* Items::instance = nullptr;

Items::Items() { this->RandomizeItemMap(); }

Items* Items::GetInstance() {
    if (instance == nullptr) {
        instance = new Items;
    }
    return instance;
}

bool Items::IsValid(uint16_t itemno) {
    if (itemno > ITEM_COUNT || itemno < 1) {
        return false;
    }
    // Items ??? blacklist
    if (itemno == 114) {
        return false;
    }
    if (itemno == 120) {
        return false;
    }
    if (itemno >= 120 and itemno <= 133) {
        return false;
    }
    if (itemno == 216) {
	// Exp. Share
        return false;
    }
    return true;
}

void Items::RandomizeItemMap() {
    int32_t seed = PlayerWork__get_uniqueID(0);

    // create map
    for (uint16_t x = 0; x < ITEM_COUNT; x++) {
        this->map[x] = x + 1;
    }

    Random rand = Random(seed);
    rand.ShuffleList(&this->map[0], ITEM_COUNT);
};

uint16_t Items::GetRandom(uint16_t itemno) {
    // do not randomize invalid number
    if (!IsValid(itemno)) {
        return itemno;
    }
    // return value from shuffled map
    do {
        itemno = this->map[itemno - 1];
    } while (!this->IsValid(itemno));
    return itemno;
};
