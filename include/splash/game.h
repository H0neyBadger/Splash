
#ifndef GAME_H
#define GAME_H

#include <cstddef>
#include <cstdint>

#include "il2cpp.h"
// linked by the linkerscript

extern void Pml_PokePara_Factory__FixInitSpec(Pml_PokePara_InitialSpec_o* pFixSpec, Pml_PokePara_InitialSpec_o* spec,
                                              const MethodInfo* method);
extern uint32_t Pml_Local_Random__GetValue(const MethodInfo* method);

extern uint32_t Pml_Local_Random__GetValue(uint32_t max, const MethodInfo* method);

extern uint64_t RandomGroupWork__GetRandom(int32_t no, const MethodInfo* method);

extern void RandomGroupWork__SetRandomSeed(int32_t no, uint64_t seed, const MethodInfo* method);

extern int32_t RandomGroupWork__RandomRange(int32_t min, int32_t max, const MethodInfo* method);

extern uint32_t PlayerWork__get_uniqueID(const MethodInfo* method);

extern XLSXContent_FieldEncountTable_Sheettable_o* GameManager__GetFieldEncountData(int32_t zoneid,
                                                                                    const MethodInfo* method);
extern XLSXContent_MapInfo_o* GameManager__get_mapInfo(const MethodInfo* method);
extern DPData_PLAYER_DATA_o* PlayerWork__get_playerData(const MethodInfo* method);
extern XLSXContent_TrainerTable_SheetTrainerData_o* TrainerSystem__GetTrainerData(int32_t trainerID,
                                                                                  const MethodInfo* method);
extern XLSXContent_TrainerTable_SheetTrainerPoke_o* TrainerSystem__GetTrainerPoke(int32_t trainerID,
                                                                                  const MethodInfo* method);
extern Dpr_EvScript_EvDataManager_o* Dpr_EvScript_EvDataManager__get_Instanse(const MethodInfo* method);

extern void Pml_PokePara_PokemonParam___ctor(Pml_PokePara_PokemonParam_o* __this, int32_t monsno, uint16_t level,
                                             uint64_t id, const MethodInfo* method);
extern int32_t PlayerWork__get_defaultPokeNo(const MethodInfo* method);
extern int32_t PlayerWork__get_rivalPokeNo(const MethodInfo* method);
extern int32_t PlayerWork__get_supportPokeNo(const MethodInfo* method);

int32_t GetTrainerId();

#endif  // ndef GAME_H
