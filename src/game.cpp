#include "game.h"
#include "farm.h"
#include "map.h"
#include "optional/combat.h"
#include "platform.h"
#include "player.h"
#include "shop.h"
#include "task.h"

// Game 的构造函数：载入地图并设置游戏的初始状态
Game::Game() {
    initMaps(*this);
    unlocked[MAP_CABIN] = unlocked[MAP_FARM] = true;
    unlocked[MAP_FOREST] = false;  // 森林和矿洞需要通过任务牌解锁
    unlocked[MAP_MINE] = false;
    currentMap = MAP_CABIN;
    day = 1;
    strcpy(message, "欢迎来到星露小谷！");
    running = true;
}

/*
 * 画出整帧画面（已实现，可参考它学习 drawScreen 的用法）。
 * 做法：先把整帧画面拼进一个 char 数组，最后调用一次 drawScreen 输出，这样不会闪烁。
 */
void renderGame(const Game& game) {
    const Player& p = game.player;
    const GameMap& map = game.maps[game.currentMap];
    char f[4096];  // 整帧画面先拼进这个数组
    const char* hand = handItemName(p);
    if (hand[0] == '\0') hand = "(空)";
    int n = 0;  // n 记录已经写入了多少个字节
    n += snprintf(f + n, sizeof(f) - n, "【%s】 第%d天\n", map.name, game.day);
    n += snprintf(f + n, sizeof(f) - n, "体力 %d/%d  生命 %d/%d  金币 %d  手持 %s\n",
                  p.energy, p.maxEnergy, p.hp, p.maxHp, p.coins, hand);
    for (int r = 0; r < map.rows; r++) {
        for (int c = 0; c < map.cols; c++) {
            f[n++] = (r == p.row && c == p.col) ? '@' : tileGlyph(map, r, c);
            f[n++] = ' ';  // 终端里字符比较"瘦"，加个空格让地图更接近正方形
        }
        f[n++] = '\n';
    }
    f[n] = '\0';  // 手动追加字符后，补上字符串结尾，再继续用 snprintf
    n += snprintf(f + n, sizeof(f) - n, "%s\n", game.message);
    n += snprintf(f + n, sizeof(f) - n, "移动 WASD/方向键  操作 J  交互 K  背包 B  退出 Q\n");
    snprintf(f + n, sizeof(f) - n, "@玩家 #墙 T树 O石头 o矿石 +农地 B床 N任务牌 $商店 >传送点\n");
    drawScreen(f);
}

// 让玩家向 (dRow, dCol) 方向移动一格，处理碰撞和传送
void movePlayer(Game& game, int dRow, int dCol) {
    /*
     *  TODO: 让玩家移动 (dRow, dCol) 格。
     *  1. 算出目标位置：r = game.player.row + dRow，c = game.player.col + dCol
     *  2. 用 isWalkable(map, r, c) 判断能不能走；不能走就直接 return（墙、水、树、设施都走不了）
     *  3. 用 findPortal(game.currentMap, r, c) 判断目标格是不是传送点：
     *       - 返回 NULL：不是传送点，正常移动（修改 player.row / player.col）
     *       - 不是 NULL：是传送点。先检查 game.unlocked[portal->toMap]，
     *         没解锁就提示一句并 return；已解锁就把 game.currentMap 改成 portal->toMap，
     *         玩家位置改成 portal->toRow / portal->toCol
     *  4. 别忘了更新 game.message，告诉玩家发生了什么（可选）
     */
    (void)game;
    (void)dRow;
    (void)dCol;
}

// 在床边睡觉：进入下一天，恢复体力和生命
void sleepInBed(Game& game) {
    game.day++;
    game.player.energy = game.player.maxEnergy;
    game.player.hp = game.player.maxHp;
    updateCropsOnNewDay(game);
    snprintf(game.message, MESSAGE_LEN, "休息结束，现在是第%d天。", game.day);
}

// 处理 J 键：根据手持物品执行对应操作
void handleActionJ(Game& game) {
    /*
     *  TODO: 按下 J 键：根据"手持物品"执行不同的操作
     *  先取手持物品名：const char* hand = handItemName(game.player);
     *  再用 strcmp(hand, ITEM_HOE) == 0 这样的写法判断（字符串不能直接用 == 比较！）
     *    锄头           -> tillSoil(game)
     *    萝卜/南瓜种子   -> plantSeed(game, hand)
     *    斧头           -> chopTree(game)
     *    镐子           -> mineResource(game)
     *    剑（选做）     -> 用 findAdjacent 找周围的怪物 'M'，找到了就 startBattle(game, r, c)
     *  提示：每次操作前可以先清空上一条提示 strcpy(game.message, "");
     */
    (void)game;
}

// 处理 K 键：与周围的设施交互
void handleActionK(Game& game) {
    /*
     *  TODO: 按下 K 键：与玩家周围四格的设施交互（不需要手持物品）
     *  用 findAdjacent(map, game.player.row, game.player.col, '字符', r, c) 依次检查：
     *    'B' 床     -> sleepInBed(game)（已经写好了，直接调用）
     *    'N' 任务牌 -> openTaskBoard(game)
     *    '$' 商店   -> openShop(game)
     *  都不是的话，尝试收获成熟的作物 -> harvestCrop(game)
     */
    (void)game;
}

// 游戏主循环，除非玩家选择退出，否则不会结束
void runGame(Game& game) {
    while (game.running) {
        renderGame(game);
        switch (readKey()) {
            case KEY_UP: movePlayer(game, -1, 0); break;
            case KEY_DOWN: movePlayer(game, 1, 0); break;
            case KEY_LEFT: movePlayer(game, 0, -1); break;
            case KEY_RIGHT: movePlayer(game, 0, 1); break;
            case KEY_J: handleActionJ(game); break;
            case KEY_K: handleActionK(game); break;
            case KEY_B: openBackpack(game); break;
            case KEY_Q: game.running = false; break;
            default: break;
        }
    }
}
