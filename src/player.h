#pragma once
#include "types.h"

// 背包基础操作（已实现，可直接使用）
bool addItem(Player& player, const char* name, int count);
bool removeItem(Player& player, const char* name, int count);  // 数量不足时返回 false 且不修改背包
int countItem(const Player& player, const char* name);
const char* handItemName(const Player& player);  // 手上没有东西时返回 ""

// 体力足够则扣除并返回 true，否则返回 false
bool spendEnergy(Player& player, int cost);

// TODO: 背包界面（按 B 打开）
void openBackpack(Game& game);
