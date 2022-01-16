#include "splash/hooks.hpp"

#include "splash/items.hpp"
#include "splash/pokemons.hpp"
#include "splash/tables.hpp"

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
    int32_t trainer_id = PlayerWork__get_uniqueID(0);
    if (trainer_id == 0) {
        // the trainer id is not initialized yet
        return;
    }

    Pokemons* pokemon = Pokemons::GetInstance();
    Items* item = Items::GetInstance();

    // randomize encouter
    XLSXContent_MapInfo_o* map_info = GameManager__get_mapInfo(0);
    XLSXContent_MapInfo_SheetZoneData_array* zone_data = map_info->fields.ZoneData;

    il2cpp_array_size_t len = zone_data->max_length;
    for (uint32_t idx; idx < len; idx++) {
        int32_t zone_id = zone_data->m_Items[idx]->fields.ZoneID;
        pokemon->RandomizeGameManagerEncountData(zone_id);
    }

    // randomize trainers
    for (uint16_t idx = 706; idx > 0; idx--) {
        pokemon->RandomizeTrainerPoke(idx);
    }

    // randomize field items
    Dpr_EvScript_EvDataManager_o* ev_instance = Dpr_EvScript_EvDataManager__get_Instanse(0);
    Dpr_EvScript_EvScriptData_array* event_list = ev_instance->fields._eventList;
    len = event_list->max_length;

    for (uint32_t idx = 0; idx < len; idx++) {
        if (event_list->m_Items[idx] == 0) {
            continue;
        }

        if (event_list->m_Items[idx]->fields.EvData == 0) {
            continue;
        }

        if (event_list->m_Items[idx]->fields.EvData->fields.Scripts == 0) {
            continue;
        }

        EvData_Script_array* scripts = event_list->m_Items[idx]->fields.EvData->fields.Scripts->fields._items;
        il2cpp_array_size_t slen = scripts->max_length;

        for (uint32_t sidx = 0; sidx < slen; sidx++) {
            EvData_Command_array* commands = scripts->m_Items[sidx]->fields.Commands->fields._items;
            il2cpp_array_size_t clen = commands->max_length;

            for (uint32_t cidx = 0; cidx < clen; cidx++) {
                EvData_Aregment_array* args = commands->m_Items[cidx]->fields.Arg->fields._items;
                il2cpp_array_size_t alen = args->max_length;

                if (alen <= 0) {
                    continue;
                }

                int32_t arg_type = args->m_Items[0].fields.argType;
                if (arg_type != 0) {
                    // unknown command
                    continue;
                }
                int32_t data = args->m_Items[0].fields.data;
                switch (data) {
                    case 187:  // EvMacro_FLD_ITEM_EVEN
                        if (args->m_Items[1].fields.argType == 1) {
                            // type arg == 1 ? is float stored in int
                            float* item_no = (float*)&(args->m_Items[1].fields.data);
                            *item_no = (float)item->GetRandom((uint16_t)*item_no);
                        }
                        break;
                }
            }
        }
    }
    return;
}

extern "C" void hook_Pml_PokePara_PokemonParam___ctor(Pml_PokePara_PokemonParam_o* __this, int32_t monsno,
                                                      uint16_t level, uint64_t id, const MethodInfo* method) {
    // hook hardcoded pokemons
    if (monsno != 0) {
        Pokemons* pokemon = Pokemons::GetInstance();
        monsno = pokemon->GetRandom((uint16_t)monsno, (uint8_t)level);
    }
    Pml_PokePara_PokemonParam___ctor(__this, monsno, level, id, method);
}

extern "C" int32_t hook_PlayerWork__get_defaultPokeNo(const MethodInfo* method) {
    int32_t monsno = PlayerWork__get_defaultPokeNo(method);
    if (monsno != 0) {
        Pokemons* pokemon = Pokemons::GetInstance();
        return pokemon->GetRandom((uint16_t)monsno, 5);
    }
    return monsno;
}

extern "C" int32_t hook_PlayerWork__get_rivalPokeNo(const MethodInfo* method) {
    int32_t monsno = PlayerWork__get_rivalPokeNo(method);
    if (monsno != 0) {
        Pokemons* pokemon = Pokemons::GetInstance();
        return pokemon->GetRandom((uint16_t)monsno, 5);
    }
    return monsno;
}

extern "C" int32_t hook_PlayerWork__get_supportPokeNo(const MethodInfo* method) {
    int32_t monsno = PlayerWork__get_supportPokeNo(method);
    if (monsno != 0) {
        Pokemons* pokemon = Pokemons::GetInstance();
        return pokemon->GetRandom((uint16_t)monsno, 5);
    }
    return monsno;
}
