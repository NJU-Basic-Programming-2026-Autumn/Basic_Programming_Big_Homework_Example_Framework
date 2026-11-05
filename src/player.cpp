#include "player.h"
#include "platform.h"

// Player 的构造函数：设置初始体力、金币和初始背包物品
Player::Player() {
    row = START_ROW;
    col = START_COL;
    maxEnergy = INIT_ENERGY;
    energy = maxEnergy;
    maxHp = INIT_HP;
    hp = maxHp;
    coins = INIT_COINS;
    itemCount = 0;
    handIndex = 0;
    addItem(*this, ITEM_HOE, 1);
    addItem(*this, ITEM_PICKAXE, 1);
    addItem(*this, ITEM_AXE, 1);
    addItem(*this, ITEM_TURNIP_SEED, 4);
}

// 往背包里加入 count 个物品，已有则累加；背包满了返回 false
bool addItem(Player& player, const char* name, int count) {
    for (int i = 0; i < player.itemCount; i++) {
        if (strcmp(player.bag[i].name, name) == 0) {
            player.bag[i].count += count;
            return true;
        }
    }
    if (player.itemCount >= MAX_ITEMS) return false;
    strncpy(player.bag[player.itemCount].name, name, NAME_LEN - 1);
    player.bag[player.itemCount].name[NAME_LEN - 1] = '\0';
    player.bag[player.itemCount].count = count;
    player.itemCount++;
    return true;
}

// 从背包里扣除 count 个物品；数量不足返回 false 且不修改背包
bool removeItem(Player& player, const char* name, int count) {
    for (int i = 0; i < player.itemCount; i++) {
        if (strcmp(player.bag[i].name, name) != 0) continue;
        if (player.bag[i].count < count) return false;
        player.bag[i].count -= count;
        if (player.bag[i].count == 0) {  // 用完了就把后面的物品往前挪一格
            for (int j = i; j < player.itemCount - 1; j++) player.bag[j] = player.bag[j + 1];
            player.itemCount--;
            if (player.handIndex >= player.itemCount) player.handIndex = player.itemCount - 1;
            if (player.handIndex < 0) player.handIndex = 0;
        }
        return true;
    }
    return false;
}

// 返回背包里某个物品的数量
int countItem(const Player& player, const char* name) {
    for (int i = 0; i < player.itemCount; i++)
        if (strcmp(player.bag[i].name, name) == 0) return player.bag[i].count;
    return 0;
}

// 返回当前手持物品的名称，手上没有东西时返回空串
const char* handItemName(const Player& player) {
    if (player.handIndex < 0 || player.handIndex >= player.itemCount) return "";
    return player.bag[player.handIndex].name;
}

// 体力足够则扣除并返回 true，否则返回 false
bool spendEnergy(Player& player, int cost) {
    if (player.energy < cost) return false;
    player.energy -= cost;
    return true;
}

/*
 * TODO : 完成背包界面
 *  1. 用 ↑/↓ (KEY_UP / KEY_DOWN) 移动光标，改变 game.player.handIndex
 *  2. 按 Enter / B / ESC 退出背包
 *  3. （进阶）在背包里按某个键吃掉食物，恢复体力
 * 下面给出的是一个只能"查看"的示例，展示了 drawScreen / readKey 的用法
 */
void openBackpack(Game& game) {
    const Player& p = game.player;
    char frame[1024];
    int n = snprintf(frame, sizeof(frame), "===== 背包 =====\n");
    for (int i = 0; i < p.itemCount; i++)
        n += snprintf(frame + n, sizeof(frame) - n, "%s%s x%d\n", i == p.handIndex ? "-> " : "   ", p.bag[i].name, p.bag[i].count);
    snprintf(frame + n, sizeof(frame) - n, "\n按任意键返回\n");
    drawScreen(frame);
    readKey();
}
