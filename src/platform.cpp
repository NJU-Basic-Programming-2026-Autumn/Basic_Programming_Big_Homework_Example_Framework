/*
 * 跨平台终端封装（Windows / macOS / Linux）。
 * 这个文件的内容不需要掌握，无需理解，学会调用文件内的接口即可
 */

#include "platform.h"
#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;

static bool g_started = false;

static void getTerminalSize(int& rows, int& cols);  // 各平台分别实现

/*
 * 整帧画面的预处理：去掉末尾换行（避免恰好占满终端时多滚一行）；
 * 如果终端放不下整帧画面，就换成一条"请放大窗口"的提示，防止画面滚动、残留
 */
static string prepareFrame(const char* frameText) {
    string frame = frameText;
    while (!frame.empty() && frame[frame.size() - 1] == '\n') frame.erase(frame.size() - 1);
    int lines = 1, width = 0, maxWidth = 0;
    for (size_t i = 0; i < frame.size(); i++) {
        unsigned char ch = (unsigned char)frame[i];
        if (ch == '\n') { lines++; width = 0; continue; }
        if ((ch & 0xC0) == 0x80) continue;  // UTF-8 后续字节不占列
        width += ch >= 0x80 ? 2 : 1;        // 汉字占两列
        if (width > maxWidth) maxWidth = width;
    }
    int rows = 0, cols = 0;
    getTerminalSize(rows, cols);
    if (rows <= 0 || cols <= 0 || (lines <= rows && maxWidth < cols)) return frame;
    char buf[200];
    snprintf(buf, sizeof(buf), "终端窗口太小，请放大。\n需要 %d 行 %d 列\n当前 %d 行 %d 列\n放大后按任意键", lines, maxWidth + 1, rows, cols);
    return buf;
}

#ifdef _WIN32
static bool g_useVT = true;  // 终端是否支持 ANSI 转义序列
// ---------------------------------------------------------------- Windows
#include <conio.h>
#include <windows.h>

// 程序结束时恢复终端（光标、输入模式、屏幕）
void platformShutdown() {
    if (!g_started) return;
    g_started = false;
    if (g_useVT) { fputs("\033[?1049l", stdout); fflush(stdout); }
    CONSOLE_CURSOR_INFO ci = {100, TRUE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

// 程序开始时初始化终端（隐藏光标、关闭回显、切换屏幕）
void platformInit() {
    if (g_started) return;
    g_started = true;
    SetConsoleOutputCP(CP_UTF8);
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    g_useVT = GetConsoleMode(out, &mode) && SetConsoleMode(out, mode | 0x0004);  // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(out, &ci);
    atexit(platformShutdown);
    if (g_useVT) fputs("\033[?1049h\033[?1007l", stdout);  // 切换到备用屏幕，退出后恢复原来的终端内容
    clearScreen();
}

void clearScreen() { system("cls"); }

// 获取终端当前的行数和列数
static void getTerminalSize(int& rows, int& cols) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) return;
    rows = info.srWindow.Bottom - info.srWindow.Top + 1;
    cols = info.srWindow.Right - info.srWindow.Left + 1;
}

// 把一整帧画面覆盖到屏幕上（不闪烁）
void drawScreen(const char* frameText) {
    string frame = prepareFrame(frameText);
    if (!g_useVT) {  // 老版本控制台：退化为 cls（会闪烁，但功能正常）
        system("cls");
        fputs(frame.c_str(), stdout);
        fflush(stdout);
        return;
    }
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
    string out;
    for (size_t i = 0; i < frame.size(); i++) {
        if (frame[i] == '\n') out += "\033[K";  // 擦掉本行剩余的旧内容
        out += frame[i];
    }
    out += "\033[J";  // 擦掉画面下方的旧内容
    fputs(out.c_str(), stdout);
    fflush(stdout);
}

// 阻塞读取一个按键，并转换成 Key 枚举
Key readKey() {
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        switch (_getch()) {
            case 72: return KEY_UP;
            case 80: return KEY_DOWN;
            case 75: return KEY_LEFT;
            case 77: return KEY_RIGHT;
        }
        return KEY_OTHER;
    }
    switch (ch) {
        case 'w': case 'W': return KEY_UP;
        case 's': case 'S': return KEY_DOWN;
        case 'a': case 'A': return KEY_LEFT;
        case 'd': case 'D': return KEY_RIGHT;
        case 'j': case 'J': return KEY_J;
        case 'k': case 'K': return KEY_K;
        case 'b': case 'B': return KEY_B;
        case 'q': case 'Q': return KEY_Q;
        case '\r': case '\n': return KEY_ENTER;
        case 27: return KEY_ESC;
    }
    return KEY_OTHER;
}

#else
// ------------------------------------------------------- macOS / Linux
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static struct termios g_oldTerm;

// 程序结束时恢复终端（光标、输入模式、屏幕）
void platformShutdown() {
    if (!g_started) return;
    g_started = false;
    tcsetattr(STDIN_FILENO, TCSANOW, &g_oldTerm);
    fputs("\033[?1006l\033[?1000l\033[?25h\033[?1049l", stdout);  // 关闭鼠标上报、恢复光标、离开备用屏幕
    fflush(stdout);
}

// 程序开始时初始化终端（隐藏光标、关闭回显、切换屏幕）
void platformInit() {
    if (g_started) return;
    g_started = true;
    tcgetattr(STDIN_FILENO, &g_oldTerm);
    struct termios raw = g_oldTerm;
    raw.c_lflag &= ~(ICANON | ECHO);  // 不等回车、不回显（保留 Ctrl+C）
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    atexit(platformShutdown);
    /*
     * 切换到备用屏幕（不留滚动记录），隐藏光标。
     */
    fputs("\033[?1049h\033[?25l\033[?1000h\033[?1006h", stdout);
    clearScreen();
}

// 彻底清屏
void clearScreen() {
    fputs("\033[2J\033[H", stdout);
    fflush(stdout);
}

// 获取终端当前的行数和列数
static void getTerminalSize(int& rows, int& cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0) return;
    rows = ws.ws_row;
    cols = ws.ws_col;
}

// 把一整帧画面覆盖到屏幕上（不闪烁）
void drawScreen(const char* frameText) {
    string frame = prepareFrame(frameText);
    string out = "\033[H";  // 光标回到左上角，直接覆盖旧画面
    for (size_t i = 0; i < frame.size(); i++) {
        if (frame[i] == '\n') out += "\033[K";
        out += frame[i];
    }
    out += "\033[J";
    fputs(out.c_str(), stdout);
    fflush(stdout);
}

// 在 ms 毫秒内尝试读一个字节，成功返回 true
static bool readByteTimeout(char& c, int ms) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    struct timeval tv = {0, ms * 1000};
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0) return false;
    return read(STDIN_FILENO, &c, 1) == 1;
}

// 把普通字符按键转换成 Key 枚举
static Key decodeChar(char c) {
    switch (c) {
        case 'w': case 'W': return KEY_UP;
        case 's': case 'S': return KEY_DOWN;
        case 'a': case 'A': return KEY_LEFT;
        case 'd': case 'D': return KEY_RIGHT;
        case 'j': case 'J': return KEY_J;
        case 'k': case 'K': return KEY_K;
        case 'b': case 'B': return KEY_B;
        case 'q': case 'Q': return KEY_Q;
        case '\r': case '\n': return KEY_ENTER;
    }
    return KEY_OTHER;
}

// 阻塞读取一个按键，并转换成 Key 枚举
Key readKey() {
    for (;;) {
        char c;
        if (read(STDIN_FILENO, &c, 1) != 1) return KEY_NONE;
        if (c != 27) return decodeChar(c);
        char c2, c3;  // 方向键会发送 ESC [ A/B/C/D
        if (!readByteTimeout(c2, 30)) return KEY_ESC;
        if (c2 != '[' || !readByteTimeout(c3, 30)) return KEY_OTHER;
        switch (c3) {
            case 'A': return KEY_UP;
            case 'B': return KEY_DOWN;
            case 'C': return KEY_RIGHT;
            case 'D': return KEY_LEFT;
            case '<':  // 鼠标/滚轮事件：ESC [ < 数字;数字;数字 M 或 m，直接丢弃
                while (readByteTimeout(c, 30) && c != 'M' && c != 'm') {}
                continue;
            case 'M':  // 旧式鼠标事件：后面跟 3 个字节，丢弃
                readByteTimeout(c, 30); readByteTimeout(c, 30); readByteTimeout(c, 30);
                continue;
        }
        return KEY_OTHER;
    }
}
#endif
