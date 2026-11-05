#include "game.h"
#include "platform.h"

// 程序入口
int main() {
    platformInit();
    Game game;
    runGame(game);
    platformShutdown();
    return 0;
}
