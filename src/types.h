#pragma once
#include <cstdio>
#include <cstring>

const int MAX_ROWS = 16;      // 地图的行数
const int MAX_COLS = 32;      // 地图的列数
const int MAX_ITEMS = 32;     // 背包内的最大物品数
const int NAME_LEN = 32;      // 名字（物品、作物、地图）最多存 31 个字节，汉字在 UTF-8 下占 3 字节
const int MESSAGE_LEN = 128;  // 提示信息的最大长度

// 游戏的初始设定（构造函数会使用这些值）
const int INIT_ENERGY = 30;   // 初始体力
const int INIT_HP = 40;       // 初始生命值
const int INIT_COINS = 80;    // 初始金币
const int START_ROW = 6;      // 游戏开始时玩家在小屋里的位置
const int START_COL = 7;

// 上、下、左、右四个方向的行/列偏移，"周围四格"可以用 for (i = 0; i < 4; i++) 遍历
const int DR[4] = {-1, 1, 0, 0};
const int DC[4] = {0, 0, -1, 1};

// 物品名称
const char* const ITEM_HOE = "锄头";
const char* const ITEM_AXE = "斧头";
const char* const ITEM_PICKAXE = "镐子";
const char* const ITEM_SWORD = "剑";
const char* const ITEM_TURNIP_SEED = "萝卜种子";
const char* const ITEM_PUMPKIN_SEED = "南瓜种子";
const char* const ITEM_TURNIP = "萝卜";
const char* const ITEM_PUMPKIN = "南瓜";
const char* const ITEM_WOOD = "木材";
const char* const ITEM_STONE = "石头";
const char* const ITEM_ORE = "矿石";

enum MapId { MAP_CABIN, MAP_FARM, MAP_FOREST, MAP_MINE, MAP_COUNT };

// 农地状态：未耕地 -> 空耕地 -> 作物生长中 -> 作物成熟 -> 空耕地
enum LandState { NOT_FARM, UNTILLED, EMPTY_FARMLAND, GROWING_CROP, RIPE_CROP };

/*
 * ============================================================================
 * 【你会发现在下面所有的结构体里都有一些函数，称为"构造函数"——这部分代码你不需要看懂，直接跳过即可】
 * 【但为了程序的完整性，我们还是简单的介绍一下】
 *
 * 构造函数是和结构体同名、没有返回值的特殊函数。每当你定义一个该结构体的变量
 * （例如 `Player player;` 或 `Game game;`），程序会自动调用它一次，
 * 把里面的成员设置成"我们规定的默认值"。
 *
 * 所以你不需要手动初始化：
 *   Game game;   // 定义完就已经是一个可以直接运行的游戏：
 *                // 4 张地图已载入，玩家体力 30、金币 80、背包里有初始物品，第 1 天
 * 你可以适当改动构造参数内的初始化值，但最好请不要改动函数体内的形式参数，以免出现语法错误。
 *
 * 构造函数的作用：
 * C++ 里局部变量不会自动清零，没有构造函数的话，成员里装的是内存里
 * 残留的随机值（比如 energy 可能是 -83921），很容易引发难以排查的 bug。
 *
 * 你自己新增结构体时，也建议写上构造函数（可参考下面的写法）。
 * 关于构造函数等面向对象编程的知识，你将会在后续课程（如高级程序设计）中学习到
 * ============================================================================
 */

// 农地结构体
struct FarmLand {
    LandState state;            // 农地状态
    char cropName[NAME_LEN];    // 作物名称
    int plantedDay;             // 播种当天的 game.day
    int matureDays;             // 作物成熟所需时间

    FarmLand() {                // 构造函数（无需了解）
        state = NOT_FARM;
        cropName[0] = '\0';
        plantedDay = 0;
        matureDays = 0;
    }
};

// 物品结构体
struct Item {
    char name[NAME_LEN];        // 物品名称
    int count;                  // 数量

    Item(const char* itemName = "", int itemCount = 0) {  // 构造函数（无需了解），参数都有缺省值
        strncpy(name, itemName, NAME_LEN - 1);
        name[NAME_LEN - 1] = '\0';
        count = itemCount;
    }
};

struct Player {
    int row, col;               // 位置（用坐标标记）
    int energy, maxEnergy;      // 当前体力值，最大体力值
    int hp, maxHp;              // 当前血量，最大血量
    int coins;                  // 所持金币数
    Item bag[MAX_ITEMS];        // 背包
    int itemCount;              // 背包里已有的物品种类数
    int handIndex;              // 当前手持物品在 bag 中的下标

    Player();                   // 构造函数（无需了解），实现在 player.cpp
};

struct GameMap {
    char name[NAME_LEN];                // 地图名称
    int rows, cols;                     // 地图实际的行数、列数
    char tiles[MAX_ROWS][MAX_COLS];     // 每个格子的地形字符
    FarmLand land[MAX_ROWS][MAX_COLS];  // 每个格子对应的农地数据（只有 '+' 格子有意义）

    GameMap() {                 // 构造函数（无需了解）
        name[0] = '\0';
        rows = MAX_ROWS;
        cols = MAX_COLS;
        for (int r = 0; r < MAX_ROWS; r++)
            for (int c = 0; c < MAX_COLS; c++) tiles[r][c] = '.';
    }
};

struct Portal {
    int fromMap, fromRow, fromCol;      // 传送点所在的地图和坐标
    int toMap, toRow, toCol;            // 传送到的地图和坐标

    Portal(int fm = 0, int fr = 0, int fc = 0, int tm = 0, int tr = 0, int tc = 0) {  // 构造函数（无需了解）
        fromMap = fm; fromRow = fr; fromCol = fc;
        toMap = tm; toRow = tr; toCol = tc;
    }
};

struct Game {
    Player player;
    GameMap maps[MAP_COUNT];
    bool unlocked[MAP_COUNT];   // 地图是否已解锁
    int currentMap;             // 当前所在的地图编号
    int day;                    // 当前是第几天
    char message[MESSAGE_LEN];  // 显示在地图下方的提示信息
    bool running;               // 游戏是否还在运行

    Game();                     // 构造函数（无需了解），实现在 game.cpp
};
