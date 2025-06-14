#ifndef chess_game_h
#define chess_game_h
#include "machine.h"
int chess_game(int chess_board_size, int join);                                   // 黑赢返回1，平局返回0，黑输返回-1，返回-404表示异常
int click_chess(int chess_board[], int chess_board_size, char *color,int *log);                                 // 人下棋
int check(int chess_board[], int chess_board_size, int x, int y);                                      // 检查双三、双四和长连
int check_three(int chess_board[], int chess_board_size, int x, int y);                                      // 检测三三禁手，如果违规返回1，否则返回0
int check_three_pattern(int chess_board[], int size, int x, int y, int dx, int dy, int color);                          // 辅助函数：检测特定方向上的三子模式
int check_continuous_three(int positions[], int valid[], int color);                                  // 检测连三模式
int check_jump_three(int positions[], int valid[], int color);                                  // 检测跳三模式
int check_four(int chess_board[], int chess_board_size, int x, int y);                                      // 检测四四禁手，如果违规返回1，否则返回0
int check_four_pattern(int chess_board[], int size, int x, int y, int dx, int dy, int color);                          // 辅助函数：检测特定方向上的四子模式
int check_live_four(int positions[], int valid[], int color);                                  // 检测活四模式
int check_rush_four(int positions[], int valid[], int color);                                  // 检测冲四模式
int long_link(int chess_board[], int size, int x, int y);                                      // 长连返回1，反之返回0
void save_final_log(int log[]);                                                                // 游戏结束时一次性保存日志文件
void record_move(int log[], int x, int y);                                                     // 仅记录棋步，不保存文件
int check_win(int chess_board[], int size, int x, int y);                                      // 检测五连，如是，返回1，否则返回0
#endif/* chess_game_h */