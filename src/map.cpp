#include "map.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// 一种简单的实现方式，直接初始化整个二维数组，不完美的实现，仅供参考
// 若你想修改地图，直接改下面的字符画即可（但同一张地图每行长度必须相同，否则启动时会报错）
static const char* CABIN[] = {
    "################",
    "#B...........N$#",
    "#..............#",
    "#..............#",
    "#..............#",
    "#..............#",
    "#..............#",
    "#######>########",
};

static const char* FARM[] = {
    "###########>############",
    "#......................#",
    "#..+++....+++..........#",
    "#..+++....+++..........#",
    "#......................#",
    "#..+++....+++..........#",
    "#......................>",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "########################",
};

static const char* FOREST[] = {
    "########################",
    "#..T..O......T.....T...#",
    "#......T.........O.....#",
    "#..O.............T.....#",
    "#......................#",
    "#...T.........O........#",
    ">......................#",
    "#.....T.......T........#",
    "#..........O...........#",
    "#.T.............T...O..#",
    "#......................>",
    "########################",
};

static const char* MINE[] = {
    "####################",
    "#..o......M....o...#",
    "#..................#",
    "#....o.........o...#",
    "#..................#",
    "#.........M........#",
    "#..o...............#",
    "#..................#",
    ">..................#",
    "####################",
};

// 传送点：从 (fromMap, fromRow, fromCol) 走到 '>' 上，会被送到 (toMap, toRow, toCol)
static const Portal PORTALS[] = {
    {MAP_CABIN, 7, 7, MAP_FARM, 1, 11},
    {MAP_FARM, 0, 11, MAP_CABIN, 6, 7},
    {MAP_FARM, 6, 23, MAP_FOREST, 6, 1},
    {MAP_FOREST, 6, 0, MAP_FARM, 6, 22},
    {MAP_FOREST, 10, 23, MAP_MINE, 8, 1},
    {MAP_MINE, 8, 0, MAP_FOREST, 10, 22},
};
static const int PORTAL_COUNT = sizeof(PORTALS) / sizeof(PORTALS[0]);

// 加载地图集
static void loadMap(GameMap& map, const char* name, const char* const rows[], int rowCount) {
    strcpy(map.name, name);
    map.rows = rowCount;
    map.cols = (int)strlen(rows[0]);
    if (rowCount > MAX_ROWS || map.cols > MAX_COLS) {
        printf("地图 %s 太大了，请调大 types.h 里的 MAX_ROWS / MAX_COLS\n", name);
        exit(1);
    }
    for (int r = 0; r < rowCount; r++) {
        if ((int)strlen(rows[r]) != map.cols) {
            printf("地图 %s 第 %d 行长度不一致\n", name, r);
            exit(1);
        }
        for (int c = 0; c < map.cols; c++) {
            map.tiles[r][c] = rows[r][c];
            FarmLand& land = map.land[r][c];
            land.state = rows[r][c] == '+' ? UNTILLED : NOT_FARM;
            land.cropName[0] = '\0';
            land.plantedDay = 0;
            land.matureDays = 0;
        }
    }
}

#define LOAD(id, name, data) loadMap(game.maps[id], name, data, sizeof(data) / sizeof(data[0]))

// 初始化地图集
void initMaps(Game& game) {
    LOAD(MAP_CABIN, "小屋", CABIN);
    LOAD(MAP_FARM, "农场", FARM);
    LOAD(MAP_FOREST, "森林", FOREST);
    LOAD(MAP_MINE, "矿洞", MINE);
}

// 是否在地图内
bool inBounds(const GameMap& map, int row, int col) {
    return row >= 0 && row < map.rows && col >= 0 && col < map.cols;
}

// 该格子是否可以走到
bool isWalkable(const GameMap& map, int row, int col) {
    if (!inBounds(map, row, col)) return false;
    return strchr("#~TOoMBN$", map.tiles[row][col]) == NULL;
}

// 返回某格在屏幕上显示的字符（农地会随状态变化）
char tileGlyph(const GameMap& map, int row, int col) {
    char t = map.tiles[row][col];
    if (t != '+') return t;
    switch (map.land[row][col].state) {
        case EMPTY_FARMLAND: return '=';
        case GROWING_CROP: return 's';
        case RIPE_CROP: return '*';
        default: return '+';
    }
}

// 在上下左右四格中寻找指定字符的格子，找到就返回 true 并输出坐标
bool findAdjacent(const GameMap& map, int row, int col, char tile, int& outRow, int& outCol) {
    for (int i = 0; i < 4; i++) {
        int r = row + DR[i], c = col + DC[i];
        if (inBounds(map, r, c) && map.tiles[r][c] == tile) {
            outRow = r;
            outCol = c;
            return true;
        }
    }
    return false;
}

// 如果该位置是传送点，返回对应的 Portal，否则返回 NULL
const Portal* findPortal(int mapId, int row, int col) {
    for (int i = 0; i < PORTAL_COUNT; i++)
        if (PORTALS[i].fromMap == mapId && PORTALS[i].fromRow == row && PORTALS[i].fromCol == col)
            return &PORTALS[i];
    return NULL;
}
