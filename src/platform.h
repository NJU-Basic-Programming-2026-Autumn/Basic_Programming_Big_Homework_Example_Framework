#pragma once

/*
 * 跨平台终端封装（Windows / macOS / Linux）。
 * 这个文件的内容不需要掌握，无需理解，学会调用文件内的接口即可
 */

enum Key {
    KEY_NONE,
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,  // W/A/S/D 与方向键都会被转换成这四个
    KEY_J, KEY_K, KEY_B, KEY_Q,
    KEY_ENTER, KEY_ESC,
    KEY_OTHER
};

void platformInit();      // 程序开始时调用一次：关闭回显、隐藏光标
void platformShutdown();  // 程序结束前调用（也会在退出时自动调用）

// 阻塞读取一个按键，不需要按回车，也不会回显
Key readKey();

// 把一整帧画面（含 '\n' 的字符串）覆盖到屏幕上，不会闪烁
void drawScreen(const char* frame);

// 彻底清屏
void clearScreen();