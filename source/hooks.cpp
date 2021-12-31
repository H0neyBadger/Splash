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
