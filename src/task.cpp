#include "task.h"
#include "platform.h"
#include "player.h"

/*
 * TODO : 完成任务牌
 *  1. 定义任务结构体 struct Task { char description[64]; bool completed; ... };
 *  2. 任务 1：金币 >= 150 时，game.unlocked[MAP_FOREST] = true，并把任务标记为已完成
 *  3. （可选）新增一个任务来解锁 MAP_MINE
 *  4. 显示每个任务的描述和完成情况
 * 下面是占位实现
 */
void openTaskBoard(Game& game) {
    drawScreen("===== 任务牌 =====\n\n（任务功能待实现）\n\n按任意键离开\n");
    readKey();
    strcpy(game.message, "你看完了任务牌。");
}
