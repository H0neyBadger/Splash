#include <string.h>
#include <switch.h>

#include "game.h"

int32_t
hook__Dpr_Item_ItemInfo__SubItem(Dpr_Item_ItemInfo_o* __this,
                                 int32_t num,
                                 const MethodInfo* method)
{
  return 0;
}

void
hook__Pml_PokePara_Factory__FixInitSpec(Pml_PokePara_InitialSpec_o* pFixSpec,
                                        Pml_PokePara_InitialSpec_o* spec,
                                        const MethodInfo* method)
{
  uint32_t monsno = Pml_Local_Random__GetValue(494, method) + 1;
  Pml_PokePara_InitialSpec_Fields* fields_p = &(spec->fields);
  fields_p->monsno = monsno;
  Pml_PokePara_Factory__FixInitSpec(pFixSpec, spec, method);
  return;
}

int32_t
GetTrainerId()
{
  // return current user trainer id
  DPData_PLAYER_DATA_o* player_data = PlayerWork__get_playerData(0);
  return player_data->fields.mystatus.fields.id;
}

uint16_t
GetRandomPokemon(uint16_t in, uint16_t* pkms_map)
{
  // return value from shuffled map
  return pkms_map[in - 1];
}

void
randomize_MonsLv_array(MonsLv_array* mons, uint16_t* pkms_map)
{
  il2cpp_array_size_t len = mons->max_length;
  for (uint32_t idx = 0; idx < len; idx++) {
    MonsLv_Fields* obj = &(mons->m_Items[idx].fields);
    int32_t rand_mons = GetRandomPokemon(obj->monsNo, pkms_map);
    if (obj->monsNo != 0) {
      // obj->maxlv += 1;
      // obj->minlv = obj->maxlv;
      obj->monsNo = rand_mons;
    }
  }
}

void
InitPokemonMap(uint16_t* pkms_map, uint32_t len)
{
  // create map
  for (uint16_t x = 0; x < len; x++) {
    pkms_map[x] = x + 1;
  }
}

void
RandPokemonMap(uint16_t* pkms_map, uint32_t len)
{
  int32_t j, seed = PlayerWork__get_uniqueID(0);
  uint16_t swap;
  uint64_t r;
  for (uint32_t i = len; i > 1; i--) {
    // naive attempt to create randomness from trainer ID
    r = i * (uint64_t) seed;
    seed ^= r & 0xffffffff;
    seed ^= r >> 32 & 0xffffffff;

    // https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
    j = seed % i; // random in range
    swap = pkms_map[j];
    pkms_map[j] = pkms_map[i];
    pkms_map[i] = swap;
  }
}

XLSXContent_FieldEncountTable_Sheettable_o*
randomize_GameManager__EncountData(int32_t zoneid, uint16_t* pkms_map)
{
  XLSXContent_FieldEncountTable_Sheettable_o* real =
    GameManager__GetFieldEncountData(zoneid, 0);
  if (real == 0) {
    return 0;
  }
  randomize_MonsLv_array(real->fields.ground_mons, pkms_map);
  randomize_MonsLv_array(real->fields.tairyo, pkms_map);
  randomize_MonsLv_array(real->fields.day, pkms_map);
  randomize_MonsLv_array(real->fields.night, pkms_map);
  randomize_MonsLv_array(real->fields.swayGrass, pkms_map);

  randomize_MonsLv_array(real->fields.gbaRuby, pkms_map);
  randomize_MonsLv_array(real->fields.gbaSapp, pkms_map);
  randomize_MonsLv_array(real->fields.gbaEme, pkms_map);
  randomize_MonsLv_array(real->fields.gbaFire, pkms_map);
  randomize_MonsLv_array(real->fields.gbaLeaf, pkms_map);

  randomize_MonsLv_array(real->fields.water_mons, pkms_map);
  randomize_MonsLv_array(real->fields.boro_mons, pkms_map);
  randomize_MonsLv_array(real->fields.ii_mons, pkms_map);
  randomize_MonsLv_array(real->fields.sugoi_mons, pkms_map);
  return real;
}

void
hook_Initialize()
{

  uint16_t pkms_len = 493;
  uint16_t pkms_map[pkms_len];
  uint16_t* pkms_map_ptr = &pkms_map[0];
  InitPokemonMap(pkms_map_ptr, pkms_len);
  RandPokemonMap(pkms_map_ptr, pkms_len);

  XLSXContent_MapInfo_o* map_info = GameManager__get_mapInfo(0);
  XLSXContent_MapInfo_SheetZoneData_array* zone_data =
    map_info->fields.ZoneData;
  il2cpp_array_size_t len = zone_data->max_length;
  for (uint32_t idx; idx < len; idx++) {
    int32_t zone_id = zone_data->m_Items[idx]->fields.ZoneID;
    randomize_GameManager__EncountData(zone_id, pkms_map_ptr);
  }
  return;
}

int
main(int argc, char** argv)
{
  // do nothing
  return 0;
}
