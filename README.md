# StardewLite 星露小谷 —— 框架代码

这是大作业的**可选**起点：你也可以完全自己从头写。使用框架的话，平台相关的屏幕刷新和键盘读取已经写好，你只需要补全游戏逻辑。

## 编译与运行

macOS / Linux（需要在终端里运行）：

    make run

Windows（MinGW g++，在 cmd / PowerShell 中运行，源文件为 UTF-8）：

    g++ -std=c++11 src/*.cpp src/optional/*.cpp -o stardew.exe
    stardew.exe

Visual Studio（开发者命令提示符）：

    cl /utf-8 /EHsc /Fe:stardew.exe src\*.cpp src\optional\*.cpp

请不要在 IDE 自带的"输出窗口"里运行，那种窗口通常不支持清屏和单键输入，请用真正的终端或控制台窗口。

## 已经实现的内容

- 4 张地图的数据（`map.cpp`）、传送点表、碰撞判断 `isWalkable`、"周围四格"查找 `findAdjacent`。对于地图，你可以按照自己的想法去随意的修改，that's fine.
- 平台层（`platform.*`）：无闪烁的整屏刷新 `drawScreen`、单键读取 `readKey`、终端大小检测。请注意，在`platform.*` 文件内，我们存放了 `macOS` 和 `Windows` 版本的控制逻辑，利用条件编译语句开启/关闭，不同系统的同学请务必注意这一点（默认是`macOS` 环境）
- 主循环 `runGame`、结构体和构造函数（定义 `Game game;` 就得到可运行的初始状态）
- 背包基础函数 `addItem` / `removeItem` / `countItem`、体力扣除 `spendEnergy`
- 画面绘制 `renderGame`（`game.cpp`），其中演示了如何用 `snprintf` 拼画面再调用 `drawScreen`，在其他子界面时可以参考这些实现。
- 睡觉 `sleepInBed`（`game.cpp`），在其他子界面时可以参考这些实现。

## 你需要完成的内容（搜索 `TODO`）

建议按下面的顺序实现：

| 顺序 | 文件 | 功能 |
| --- | --- | --- |
| 1 | `game.cpp` `movePlayer` | 移动、碰撞、地图传送（做完就能在地图上走动） |
| 2 | `game.cpp` `handleActionJ/K` | 按键分发（J：手持物品操作，K：设施交互） |
| 3 | `farm.cpp` `tillSoil` | 耕地（后面几个函数的模板） |
| 4 | `farm.cpp` | 播种、收获、作物成长、砍树、挖矿 |
| 5 | `player.cpp` `openBackpack` | 背包界面：↑/↓ 切换手持物 |
| 6 | `shop.cpp` | 商店买卖 |
| 7 | `task.cpp` | 任务牌，金币达到 150 解锁森林；新增任务解锁矿洞 |
| 选做 | `optional/combat.cpp` | 战斗系统 |

## 代码结构

    src/
      main.cpp      程序入口
      types.h       所有结构体、枚举、常量
      platform.*    跨平台终端封装（不用修改）
      map.*         地图数据、碰撞、传送点、"周围四格"查找
      game.*        主循环、绘制、移动、按键分发
      player.*      玩家与背包
      farm.*        农地与资源采集
      shop.* task.* 商店与任务牌
      optional/     选做内容

## 几个提示

- 所有操作函数都接收 `Game& game`，直接修改里面的数据；要告诉玩家发生了什么，就修改 `game.message`（它是 `char` 数组）：
  固定文字用 `strcpy(game.message, "你开垦了一块土地。");`，带数字用 `snprintf(game.message, MESSAGE_LEN, "获得金币 %d", n);`。
- "周围四格"：用 `for (int i = 0; i < 4; i++)` 配合 `DR[i]`、`DC[i]` 遍历。
- 本框架全程使用 `char` 数组表示字符串（不使用 `string`）。比较字符串用 `strcmp(a, b) == 0`，拷贝用 `strcpy` / `strncpy`；物品名称请使用 `types.h` 里的 `ITEM_` 常量。
- 需要弹出子界面（商店、任务牌、背包）时，在函数里自己写一个循环：`drawScreen(...)` 画 -> `readKey()` 读 -> 处理 -> 满足退出条件就 `return`。
- 修改地图只需要改 `map.cpp` 开头的字符画；新增地图类型需要在 `types.h`、`map.cpp` 里各加几行。
- 想把代码拆得更细（例如新建 `inventory.cpp`）完全可以，记得同时把新文件加入编译命令（Makefile 会自动包含 `src/` 下所有 `.cpp`）。


## 字符串怎么写

在我们给出的大作业框架里，所有文字都存在 `char` 数组里。`char` 数组**不能**用 `=` 赋值、不能用 `==` 比较、不能用 `+` 拼接，要用下面这些函数（需要 `#include <cstring>`，框架的 `types.h` 已经包含了）：

| 想做的事 | 错误写法 | 正确写法 |
| --- | --- | --- |
| 赋值 | `game.message = "你好";` | `strcpy(game.message, "你好");` |
| 比较是否相同 | `if (hand == ITEM_HOE)` | `if (strcmp(hand, ITEM_HOE) == 0)` |
| 接在后面 | `a = a + b;` | `strcat(a, b);`（`a` 要有足够的空间） |
| 带数字的文字 | `"金币" + n` | `snprintf(buf, sizeof(buf), "金币%d", n);` |
| 判断是不是空串 | `if (name == "")` | `if (name[0] == '\0')` |

### snprintf：把格式化后的文字写进数组

在大作业的框架里，我们几乎没有使用`printf`或者`cout`，而都是使用`snprintf`。其用法和 `printf` 几乎一样，区别是 `printf` 输出到屏幕，`snprintf` 写进数组，并且多了前两个参数：

    char buf[64];
    snprintf(buf, sizeof(buf), "第%d天，金币%d", 3, 80);
    // buf 里现在是 "第3天，金币80"

- 第 1 个参数：写入哪个数组
- 第 2 个参数：数组的大小（用 `sizeof(数组名)`），写得太长会被自动截断，不会越界
- 后面：和 `printf` 一样，格式字符串加上要填进去的值
- 常用占位符：`%d` 整数，`%s` 字符串，`%c` 单个字符

写 `game.message` 时，大小用 `MESSAGE_LEN`：

    snprintf(game.message, MESSAGE_LEN, "获得金币 %d", n);

### 一帧画面里连续往后写

`snprintf` 会返回"写入了多少个字符"，用一个变量 `n` 记住已经写到哪里，下一次从 `f + n` 开始继续写：

    char f[1024];
    int n = 0;
    n += snprintf(f + n, sizeof(f) - n, "第%d天\n", game.day);   // 写第一行
    n += snprintf(f + n, sizeof(f) - n, "金币 %d\n", game.player.coins);  // 接着写第二行
    f[n++] = '@';          // 追加单个字符，直接放进数组
    f[n] = '\0';           // 手动追加字符后，要补上字符串结尾
    drawScreen(f);

商店、背包、任务牌这类子界面，可以参考 `game.cpp` 里的 `renderGame`，写法完全一样。

### 你可能需要注意的地方

- 汉字在 UTF-8 下占 3 个字节，`NAME_LEN` 为 32，大约只能存 10 个汉字。
- `strcpy` 不检查长度，目标数组要比源字符串大。
- 数组要有 `'\0'` 结尾才是字符串；`strcat` 之前，目标数组必须已经是一个合法字符串（比如先 `a[0] = '\0';`）。

## 在 VS Code 中运行

1. 用 VS Code 打开 `StardewLite` 这个文件夹。
3. 打开终端（Ctrl+\`），输入 `make run`（macOS）。Windows 输入：`g++ -std=c++11 src/*.cpp src/optional/*.cpp -o stardew.exe` 然后 `./stardew.exe`。

注意：不要点右上角的"Run Code"三角按钮，它只编译当前一个文件，会出现链接错误。游戏需要在终端面板里按键操作，先点一下终端面板让它获得焦点。

如果终端面板太矮（农场地图需要约 17 行），游戏会提示"终端窗口太小"。把面板拖高（或点面板右上角的最大化按钮）后按任意键即可继续。
