#include "combat.h"
#include "../platform.h"

/*
 * TODO(选做): 实现回合制战斗
 *  1. 定义 struct Monster { char name[NAME_LEN]; int hp, attack, defense, reward; };
 *  2. 循环：显示双方血量 -> 读取玩家选择（进攻/防御/吃食物）-> 怪物行动 -> 判断胜负
 *  3. 胜利：怪物所在格改为 '.'，发放金币；失败：送回小屋并恢复部分生命（参考文档）
 */
void startBattle(Game& game, int monsterRow, int monsterCol) {
    (void)monsterRow;
    (void)monsterCol;
    drawScreen("===== 战斗 =====\n\n（战斗功能待实现）\n\n按任意键返回\n");
    readKey();
    strcpy(game.message, "你离开了战斗。");
}
