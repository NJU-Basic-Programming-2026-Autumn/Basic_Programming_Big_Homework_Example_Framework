#pragma once
#include "../types.h"

/*
 * [Optional] 在怪物 'M' 周围四格手持剑按 J 时调用。
 * 战斗结束前玩家不能在地图上移动。
 */
void startBattle(Game& game, int monsterRow, int monsterCol);
