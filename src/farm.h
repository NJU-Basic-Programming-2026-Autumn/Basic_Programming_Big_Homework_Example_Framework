#pragma once
#include "types.h"

// 以下函数都对"玩家周围四格"生效，成功后修改 game.message 告诉玩家发生了什么。

// TODO: 手持锄头按 J：把周围的未耕地变成空耕地，消耗 2 体力（建议最先实现，作为后面几个函数的模板）
bool tillSoil(Game& game);

// TODO: 手持种子按 J：把周围的空耕地变成"生长中"，消耗 1 个种子
bool plantSeed(Game& game, const char* seedName);

// TODO: 按 K：收获周围成熟的作物，放进背包，农地变回空耕地
bool harvestCrop(Game& game);

// TODO: 手持斧头按 J：砍周围的树（T），消耗 2 体力，获得木材
bool chopTree(Game& game);

// TODO: 手持镐子按 J：挖周围的石头（O）或矿石（o），消耗 2 体力
bool mineResource(Game& game);

// TODO: 进入新的一天时调用，更新所有作物的生长状态（可选：也可以在收获时再根据天数判断）
void updateCropsOnNewDay(Game& game);
