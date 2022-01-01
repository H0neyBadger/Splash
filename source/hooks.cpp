#include "splash/hooks.hpp"

#include "splash/pokemons.hpp"

extern "C" int32_t hook__Dpr_Item_ItemInfo__SubItem(Dpr_Item_ItemInfo_o* __this, int32_t num,
                                                    const MethodInfo* method) {
    return 0;
}

void hook__Pml_PokePara_Factory__FixInitSpec(Pml_PokePara_InitialSpec_o* pFixSpec, Pml_PokePara_InitialSpec_o* spec,
                                             const MethodInfo* method) {
    uint32_t monsno = Pml_Local_Random__GetValue(494, method) + 1;
    Pml_PokePara_InitialSpec_Fields* fields_p = &(spec->fields);
    fields_p->monsno = monsno;
    Pml_PokePara_Factory__FixInitSpec(pFixSpec, spec, method);
    return;
}

int32_t GetTrainerId() {
    // return current user trainer id
    DPData_PLAYER_DATA_o* player_data = PlayerWork__get_playerData(0);
    return player_data->fields.mystatus.fields.id;
}

extern "C" void hook_Initialize() {
    Pokemons* pokemon = Pokemons::GetInstance();

    XLSXContent_MapInfo_o* map_info = GameManager__get_mapInfo(0);
    XLSXContent_MapInfo_SheetZoneData_array* zone_data = map_info->fields.ZoneData;
    il2cpp_array_size_t len = zone_data->max_length;
    for (uint32_t idx; idx < len; idx++) {
        int32_t zone_id = zone_data->m_Items[idx]->fields.ZoneID;
        pokemon->RandomizeGameManagerEncountData(zone_id);
    }
    return;
}

extern "C" void hook_Pml_PokePara_PokemonParam___ctor(Pml_PokePara_PokemonParam_o* __this, int32_t monsno,
                                                      uint16_t level, uint64_t id, const MethodInfo* method) {
    // hook hardcoded pokemons
    if (monsno != 0) {
        Pokemons* pokemon = Pokemons::GetInstance();
        monsno = pokemon->GetRandom((uint16_t)monsno);
    }
    Pml_PokePara_PokemonParam___ctor(__this, monsno, level, id, method);
}

extern "C" int32_t hook_PlayerWork__get_defaultPokeNo(const MethodInfo* method) {
    int32_t monsno = PlayerWork__get_defaultPokeNo(method);
    if (monsno != 0) {
        Pokemons* pokemon = Pokemons::GetInstance();
        return pokemon->GetRandom((uint16_t)monsno);
    }
    return monsno;
}

extern "C" int32_t hook_PlayerWork__get_rivalPokeNo(const MethodInfo* method) {
    int32_t monsno = PlayerWork__get_rivalPokeNo(method);
    if (monsno != 0) {
        Pokemons* pokemon = Pokemons::GetInstance();
        return pokemon->GetRandom((uint16_t)monsno);
    }
    return monsno;
}

extern "C" int32_t hook_PlayerWork__get_supportPokeNo(const MethodInfo* method) {
    int32_t monsno = PlayerWork__get_supportPokeNo(method);
    if (monsno != 0) {
        Pokemons* pokemon = Pokemons::GetInstance();
        return pokemon->GetRandom((uint16_t)monsno);
    }
    return monsno;
}
