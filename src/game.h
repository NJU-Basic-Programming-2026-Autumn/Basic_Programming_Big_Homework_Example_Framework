#pragma once
#include "types.h"
// 主循环
void runGame(Game& game);

// 画出整帧画面（我们已经帮你实现，可参考它学习 drawScreen 的用法）
void renderGame(const Game& game);

// TODO: 移动玩家，处理碰撞和传送点
void movePlayer(Game& game, int dRow, int dCol);

// 在床边按 K：睡觉，进入下一天（已实现，作为示例）
void sleepInBed(Game& game);

// TODO: J 键：根据手持物品执行对应操作；K 键：与周围的设施交互
void handleActionJ(Game& game);
void handleActionK(Game& game);
