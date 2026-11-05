#include "shop.h"
#include "platform.h"
#include "player.h"

/*
 * TODO : 完成商店
 *  1. 定义商品结构体 struct ShopItem { char name[NAME_LEN]; int buyPrice; int sellPrice; };
 *     用数组保存：买价 -1 表示不允许购买，卖价 -1 表示不允许出售
 *  2. 做一个循环：显示商品列表和金币，用 ↑/↓ 选择商品，
 *     用 B 键买入、S 键卖出（可自行设计按键），ESC 退出
 *  3. 买入：金币够 -> game.player.coins 减少，addItem
 *     卖出：removeItem 成功 -> game.player.coins 增加
 * 下面是占位实现
 */
void openShop(Game& game) {
    drawScreen("===== 商店 =====\n\n（商店功能待实现）\n\n按任意键离开\n");
    readKey();
    strcpy(game.message, "你离开了商店。");
}
