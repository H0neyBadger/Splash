
#ifndef GAME_H
#define GAME_H

#include <cstddef>
#include <cstdint>

#include "il2cpp.h"
// linked by the linkerscript

void
Pml_PokePara_Factory__FixInitSpec(Pml_PokePara_InitialSpec_o* pFixSpec,
                                  Pml_PokePara_InitialSpec_o* spec,
                                  const MethodInfo* method);
uint32_t
Pml_Local_Random__GetValue(const MethodInfo* method);

uint32_t
Pml_Local_Random__GetValue(uint32_t max, const MethodInfo* method);

uint64_t
RandomGroupWork__GetRandom(int32_t no, const MethodInfo* method);

void
RandomGroupWork__SetRandomSeed(int32_t no,
                               uint64_t seed,
                               const MethodInfo* method);

int32_t
RandomGroupWork__RandomRange(int32_t min,
                             int32_t max,
                             const MethodInfo* method);

uint32_t
PlayerWork__get_uniqueID(const MethodInfo* method);

XLSXContent_FieldEncountTable_Sheettable_o*
GameManager__GetFieldEncountData(int32_t zoneid, const MethodInfo* method);

XLSXContent_MapInfo_o*
GameManager__get_mapInfo(const MethodInfo* method);

DPData_PLAYER_DATA_o*
PlayerWork__get_playerData(const MethodInfo* method);

#endif // ndef GAME_H
