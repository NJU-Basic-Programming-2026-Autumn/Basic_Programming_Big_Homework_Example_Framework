#pragma once
#include "types.h"

/*
 * 地图字符说明：
 *  #墙  .地面  ~水  T树  O石头  o矿石  M怪物  +农地  B床  N任务牌  $商店  >传送点
 */

void initMaps(Game& game);

bool inBounds(const GameMap& map, int row, int col);

// 该格子能否走上去（墙、水、树、石头、设施等不可通行）
bool isWalkable(const GameMap& map, int row, int col);

// 返回某格在屏幕上显示的字符（农地会根据状态变化）
char tileGlyph(const GameMap& map, int row, int col);

// 在 (row, col) 的上下左右四格中寻找字符为 tile 的格子，找到则返回 true 并把坐标写入 outRow/outCol
bool findAdjacent(const GameMap& map, int row, int col, char tile, int& outRow, int& outCol);

// 如果 (mapId, row, col) 是传送点，返回对应的 Portal，否则返回 NULL
const Portal* findPortal(int mapId, int row, int col);
