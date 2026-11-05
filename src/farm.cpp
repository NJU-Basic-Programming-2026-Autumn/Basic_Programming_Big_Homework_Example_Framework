#include "farm.h"
#include "map.h"
#include "player.h"

// 手持锄头按 J：把周围的未耕地开垦成空耕地
bool tillSoil(Game& game) {
    /*
     * TODO : 手持锄头按 J：把玩家周围四格内的一块"未耕地"开垦成"空耕地"。这是后面几个函数的模板，先做它。
     *  1. 取出当前地图和玩家：GameMap& map = game.maps[game.currentMap]; Player& player = game.player;
     *  2. 遍历上下左右四个方向，找到 map.land[r][c].state == UNTILLED 的格子：
     *         for (int i = 0; i < 4; i++) {
     *             int r = player.row + DR[i], c = player.col + DC[i];
     *             if (!inBounds(map, r, c)) continue;      // 先判断越界，再访问数组
     *             ...
     *         }
     *  3. 找到后：用 spendEnergy(player, 2) 扣体力（返回 false 说明体力不足，提示并返回）；
     *     成功就把 state 改成 EMPTY_FARMLAND，写一句 game.message
     *  4. 周围没有未耕地：什么都不做，直接 return false
     */
    (void)game;
    return false;
}

// 手持种子按 J：在周围的空耕地里播种
bool plantSeed(Game& game, const char* seedName) {
    /*
     * TODO: 参考 tillSoil 的写法
     *  1. 遍历四个方向，找到 state == EMPTY_FARMLAND 的格子
     *  2. 用 removeItem(game.player, seedName, 1) 扣除种子
     *  3. 设置 state = GROWING_CROP，记录 cropName、plantedDay = game.day、matureDays = 2
     */
    (void)seedName;
    strcpy(game.message, "（播种功能待实现）");
    return false;
}

// 按 K：收获周围成熟的作物
bool harvestCrop(Game& game) {
    // TODO: 找到周围 state == RIPE_CROP 的格子，用 addItem 把作物放进背包，再把农地变回 EMPTY_FARMLAND
    strcpy(game.message, "（收获功能待实现）");
    return false;
}

// 手持斧头按 J：砍周围的树，获得木材
bool chopTree(Game& game) {
    /*
     * TODO: 用 findAdjacent(map, row, col, 'T', r, c) 找树；消耗 2 体力；获得木材
     *       想一想：树被砍掉后，地图上的 'T' 应该变成什么？
     */
    strcpy(game.message, "（砍树功能待实现）");
    return false;
}

// 手持镐子按 J：挖周围的石头或矿石
bool mineResource(Game& game) {
    // TODO: 类似 chopTree，分别处理 'O'（石头）和 'o'（矿石）
    strcpy(game.message, "（挖矿功能待实现）");
    return false;
}

// 进入新的一天时更新所有作物的生长状态
void updateCropsOnNewDay(Game& game) {
    // TODO: 遍历农场地图，把种下满 matureDays 天的作物变成 RIPE_CROP
    (void)game;
}
