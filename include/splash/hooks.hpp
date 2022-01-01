#ifndef HOOK_H
#define HOOK_H

#include "splash/game.h"

extern "C" int32_t hook__Dpr_Item_ItemInfo__SubItem(Dpr_Item_ItemInfo_o* __this, int32_t num,
                                                    const MethodInfo* method);

void hook__Pml_PokePara_Factory__FixInitSpec(Pml_PokePara_InitialSpec_o* pFixSpec, Pml_PokePara_InitialSpec_o* spec,
                                             const MethodInfo* method);

extern "C" void hook_Initialize();

extern "C" int32_t hook_PlayerWork__get_defaultPokeNo(const MethodInfo* method);
extern "C" int32_t hook_PlayerWork__get_rivalPokeNo(const MethodInfo* method);
extern "C" int32_t hook_PlayerWork__get_supportPokeNo(const MethodInfo* method);

#endif  // ndef HOOK_H
