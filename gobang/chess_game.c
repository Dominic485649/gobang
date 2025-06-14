#include "chess_game.h"
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
int chess_game(int chess_board_size, int join)
{
    SetConsoleOutputCP(65001);
    int *chess_board = (int *)calloc(chess_board_size * chess_board_size, sizeof(int)); // 二维数组定义棋盘
    int *log = (int *)calloc(400, sizeof(int));                                         // 初始化日志数组，用于记录棋子位置
    // 添加内存分配检查
    if (!chess_board || !log)
    {
        if (chess_board)
            free(chess_board);
        if (log)
            free(log);
        return -404;
    }

    log[0] = chess_board_size;
    log[1] = join; // 0表示人人对战，1表示人机对战，人先行，2表示人机对战，人后行
    log[3] = -2;   //-2表示输赢未定，1表示黑棋赢，-1表示白棋赢，0表示平局
    log[4] = 0;    // 记录当前步数
    log[5] = 0;    // 记录悔棋的总步数
    log[8] = 0;    // 纠错，如果是-404，表示log溢出
    while (1)
    {
        if (join == 0) // 人人对战
        {
            char *black = "black";
            char *white = "white";
            if (click_chess(chess_board, chess_board_size, black, log))
            {
                log[3] = -1; // -1表示白赢
                free(chess_board);
                free(log);
                return -1; // 返回-1表示黑输
            }
            if (8 + log[4] < 400)
            {
                if (check_win(chess_board, chess_board_size, log[8 + log[4]] >> 7, 127 & log[8 + log[4]]))
                {
                    log[3] = 1; // 1表示黑赢
                    save_final_log(log); // 保存最终游戏状态
                    free(chess_board);
                    free(log);
                    return 1; // 返回1表示黑赢
                }
            }

            else
            {
                log[8] = -404;
                free(chess_board);
                free(log);
                return -404;
            }
            click_chess(chess_board, chess_board_size, white, log);
            if (8 + log[4] < 400)
            {
                if (check_win(chess_board, chess_board_size, log[8 + log[4]] >> 7, 127 & log[8 + log[4]]))
                {
                    log[3] = -1; // -1表示白赢
                    save_final_log(log); // 保存最终游戏状态
                    free(chess_board);
                    free(log);
                    return -1; // 返回-1表示黑输，即表示白赢
                }
            }

            else
            {
                log[8] = -404;
                free(chess_board);
                free(log);
                return -404;
            }
        }
        else if (join == 1) // 人机对战，人先行
        {
            char *black = "black";
            if (click_chess(chess_board, chess_board_size, black, log))
            {
                log[3] = 1; // 1表示黑赢
                free(chess_board);
                free(log);
                return 1; // 返回1表示黑赢
            }
            if (8 + log[4] < 400)
            {
                if (check_win(chess_board, chess_board_size, log[8 + log[4]] >> 7, 127 & log[8 + log[4]]))
                {
                    log[3] = -1; // -1表示白赢
                    save_final_log(log); // 保存最终游戏状态
                    free(chess_board);
                    free(log);
                    return -1; // 返回-1表示黑输，即表示白赢
                }
            }

            else
            {
                log[8] = -404;
                free(chess_board);
                free(log);
                return -404;
            }
            // 机器下棋
            click_machine(chess_board, chess_board_size, log);
            if (8 + log[4] < 400)
            {
                if (check_win(chess_board, chess_board_size, log[8 + log[4]] >> 7, 127 & log[8 + log[4]]))
                {
                    log[3] = -1; // -1表示白赢
                    save_final_log(log); // 保存最终游戏状态
                    free(chess_board);
                    free(log);
                    return -1; // 返回-1表示黑输，即表示白赢
                }
            }

            else
            {
                log[8] = -404;
                free(chess_board);
                free(log);
                return -404;
            }
        }
        else if (join == 2) // 人机对战，机器先行
        {
            // 机器下棋
            click_machine(chess_board, chess_board_size, log);
            if (8 + log[4] < 400)
            {
                if (check_win(chess_board, chess_board_size, log[8 + log[4]] >> 7, 127 & log[8 + log[4]]))
                {
                    log[3] = 1; // 1表示黑赢
                    save_final_log(log); // 保存最终游戏状态
                    free(chess_board);
                    free(log);
                    return 1; // 返回1表示黑赢
                }
            }

            else
            {
                log[8] = -404;
                free(chess_board);
                free(log);
                return -404;
            }
            char *white = "white";
            click_chess(chess_board, chess_board_size, white, log);
            if (8 + log[4] < 400)
            {
                if (check_win(chess_board, chess_board_size, log[8 + log[4]] >> 7, 127 & log[8 + log[4]]))
                {
                    log[3] = -1; // -1表示白赢
                    save_final_log(log); // 保存最终游戏状态
                    free(chess_board);
                    free(log);
                    return -1; // 返回-1表示黑输，即表示白赢
                }
            }

            else
            {
                free(chess_board);
                free(log);
                return -404;
            }
        }
    }
}
int click_chess(int chess_board[], int chess_board_size, char *color, int *log)
{
    SetConsoleOutputCP(65001);
    int x, y, chess_piece;
re:
    printf("Please enter the coordinates:\n");
    scanf("%d %d", &x, &y);
    if (chess_board[x * chess_board_size + y] == 0 && strcmp(color, "black") == 0)
    {
        chess_board[x * chess_board_size + y] = 1;
        record_move(log, x, y);
    }
    else if (chess_board[x * chess_board_size + y] == 0 && strcmp(color, "white") == 0)
    {
        chess_board[x * chess_board_size + y] = -1;
        record_move(log, x, y);
    }
    else
    {
        printf("Please re-enter!\n");
        goto re;
    }

    if (strcmp(color, "black") == 0 && check(chess_board, chess_board_size, x, y)) // 校验双三、双四和长连
        return 1;                                                                  // 返回1表示违规
    else
        return 0; // 返回0表示合法
}

int check(int chess_board[], int chess_board_size, int x, int y) // 校验双三、双四和长连，如果违规返回1，否则返回0
{
    SetConsoleOutputCP(65001);
    // 调用check_three函数，校验双三
    if (check_three(chess_board, chess_board_size, x, y) || check_four(chess_board, chess_board_size, x, y) || (long_link(chess_board, chess_board_size, x, y)))
        return 1;
    else
        return 0;
}

// 检测三三禁手，如果违规返回1，否则返回0
int check_three(int chess_board[], int chess_board_size, int x, int y)
{
    int pos = x * chess_board_size + y;
    int color = chess_board[pos];

    // 只有黑棋才有禁手限制
    if (color != 1)
        return 0;

    int count = 0;

    // 四个方向：水平、垂直、正斜线、反斜线
    int dirs[4][2] = {
        {1, 0}, // 水平
        {0, 1}, // 垂直
        {1, 1}, // 正斜线
        {1, -1} // 反斜线
    };

    for (int d = 0; d < 4; d++)
    {
        int dx = dirs[d][0];
        int dy = dirs[d][1];

        // 检测该方向上的所有可能三子模式
        if (check_three_pattern(chess_board, chess_board_size, x, y, dx, dy, color))
        {
            count++;
        }
    }
    if (count >= 2)
        return 1;
    else
        return 0;
}

// 辅助函数：检测特定方向上的三子模式
int check_three_pattern(int chess_board[], int size, int x, int y, int dx, int dy, int color)
{
    // 检测以当前位置为中心的各种三子模式

    // 模式1: 连三 OXX_X (当前位置在不同位置)
    // 模式2: 跳三 OX_XX, OXX_X 等

    // 向两个方向扩展，检查7个位置的模式
    int positions[7];
    int valid[7];

    // 获取以当前位置为中心的7个位置的值
    for (int i = -3; i <= 3; i++)
    {
        int nx = x + i * dx;
        int ny = y + i * dy;

        if (nx >= 0 && nx < size && ny >= 0 && ny < size)
        {
            positions[i + 3] = chess_board[nx * size + ny];
            valid[i + 3] = 1;
        }
        else
        {
            positions[i + 3] = -2; // 边界标记
            valid[i + 3] = 0;
        }
    }

    // 检测各种三子模式
    // 当前位置在positions[3]

    // 检测连三模式
    if (check_continuous_three(positions, valid, color))
    {
        return 1;
    }

    // 检测跳三模式
    if (check_jump_three(positions, valid, color))
    {
        return 1;
    }

    return 0;
}

// 检测连三模式
int check_continuous_three(int positions[], int valid[], int color)
{
    // 检测 _XXX_ 模式（中间三个连续）
    if (valid[2] && valid[3] && valid[4] && valid[5] && valid[1] &&
        positions[1] == 0 && positions[2] == color && positions[3] == color &&
        positions[4] == color && positions[5] == 0)
    {
        return 1;
    }

    // 检测 _XX_X 和 _X_XX 等其他连三变形
    // 左侧连三：__XXX
    if (valid[0] && valid[1] && valid[2] && valid[3] && valid[4] &&
        positions[0] == 0 && positions[1] == 0 && positions[2] == color &&
        positions[3] == color && positions[4] == color)
    {
        // 检查右侧是否有空位形成活三
        if (valid[5] && positions[5] == 0)
        {
            return 1;
        }
    }

    // 右侧连三：XXX__
    if (valid[2] && valid[3] && valid[4] && valid[5] && valid[6] &&
        positions[2] == color && positions[3] == color && positions[4] == color &&
        positions[5] == 0 && positions[6] == 0)
    {
        // 检查左侧是否有空位形成活三
        if (valid[1] && positions[1] == 0)
        {
            return 1;
        }
    }

    return 0;
}

// 检测跳三模式
int check_jump_three(int positions[], int valid[], int color)
{
    // 检测 _XX_X_ 模式
    if (valid[1] && valid[2] && valid[3] && valid[4] && valid[5] &&
        positions[1] == 0 && positions[2] == color && positions[3] == color &&
        positions[4] == 0 && positions[5] == color)
    {
        // 检查两端是否有足够空间
        if (valid[0] && valid[6] && positions[0] == 0 && positions[6] == 0)
        {
            return 1;
        }
    }

    // 检测 _X_XX_ 模式
    if (valid[1] && valid[2] && valid[3] && valid[4] && valid[5] &&
        positions[1] == 0 && positions[2] == color && positions[3] == 0 &&
        positions[4] == color && positions[5] == color)
    {
        // 检查两端是否有足够空间
        if (valid[0] && valid[6] && positions[0] == 0 && positions[6] == 0)
        {
            return 1;
        }
    }

    // 检测 _X_X_X_ 模式（间隔三）
    if (valid[0] && valid[2] && valid[4] && valid[6] && valid[8] &&
        positions[0] == color && positions[2] == color &&
        positions[4] == color && positions[6] == color &&
        positions[1] == 0 && positions[3] == 0 && positions[5] == 0 &&
        positions[7] == 0)
        return 1;

    return 0;
}

// 检测四四禁手，如果违规返回1，否则返回0
int check_four(int chess_board[], int chess_board_size, int x, int y)
{
    int pos = x * chess_board_size + y;
    int color = chess_board[pos];

    // 只有黑棋才有四四禁手限制
    if (color != 1)
        return 0;

    int count = 0;

    // 四个方向：水平、垂直、正斜线、反斜线
    int dirs[4][2] = {
        {1, 0}, // 水平
        {0, 1}, // 垂直
        {1, 1}, // 正斜线
        {1, -1} // 反斜线
    };

    for (int d = 0; d < 4; d++)
    {
        int dx = dirs[d][0];
        int dy = dirs[d][1];

        // 检测该方向上的所有可能四子模式
        count += check_four_pattern(chess_board, chess_board_size, x, y, dx, dy, color);
    }
    if (count >= 2)
        return 1;
    else
        return 0;
}

// 辅助函数：检测特定方向上的四子模式
int check_four_pattern(int chess_board[], int size, int x, int y, int dx, int dy, int color)
{
    int pattern_count = 0;

    // 获取以当前位置为中心的9个位置的值（扩展范围以检测所有四子模式）
    int positions[9];
    int valid[9];

    for (int i = -4; i <= 4; i++)
    {
        int nx = x + i * dx;
        int ny = y + i * dy;

        if (nx >= 0 && nx < size && ny >= 0 && ny < size)
        {
            positions[i + 4] = chess_board[nx * size + ny];
            valid[i + 4] = 1;
        }
        else
        {
            positions[i + 4] = -2; // 边界标记
            valid[i + 4] = 0;
        }
    }

    // 当前位置在positions[4]

    // 检测活四模式
    pattern_count += check_live_four(positions, valid, color);

    // 检测冲四模式
    pattern_count += check_rush_four(positions, valid, color);

    return pattern_count;
}

// 检测活四模式
int check_live_four(int positions[], int valid[], int color)
{
    // 检测 _XXXX_ 模式（标准活四）
    if (valid[2] && valid[3] && valid[4] && valid[5] && valid[6] && valid[1] && valid[7] &&
        positions[1] == 0 && positions[2] == color && positions[3] == color &&
        positions[4] == color && positions[5] == color && positions[6] == 0)
    {
        return 1;
    }

    // 检测跳活四模式 _XXX_X_
    if (valid[1] && valid[2] && valid[3] && valid[4] && valid[5] && valid[6] && valid[7] &&
        positions[1] == 0 && positions[2] == color && positions[3] == color &&
        positions[4] == color && positions[5] == 0 && positions[6] == color && positions[7] == 0)
    {
        return 1;
    }

    // 检测跳活四模式 _XX_XX_
    if (valid[1] && valid[2] && valid[3] && valid[4] && valid[5] && valid[6] && valid[7] &&
        positions[1] == 0 && positions[2] == color && positions[3] == color &&
        positions[4] == 0 && positions[5] == color && positions[6] == color && positions[7] == 0)
    {
        return 1;
    }

    // 检测跳活四模式 _X_XXX_
    if (valid[1] && valid[2] && valid[3] && valid[4] && valid[5] && valid[6] && valid[7] &&
        positions[1] == 0 && positions[2] == color && positions[3] == 0 &&
        positions[4] == color && positions[5] == color && positions[6] == color && positions[7] == 0)
    {
        return 1;
    }

    return 0;
}

// 检测冲四模式
int check_rush_four(int positions[], int valid[], int color)
{
    int rush_count = 0;

    // 检测 XXXX_ 模式（右冲四）
    if (valid[1] && valid[2] && valid[3] && valid[4] && valid[5] &&
        positions[1] == color && positions[2] == color && positions[3] == color &&
        positions[4] == color && positions[5] == 0)
    {
        // 检查左侧是否被阻挡
        if (!valid[0] || positions[0] != 0)
        {
            rush_count++;
        }
    }

    // 检测 _XXXX 模式（左冲四）
    if (valid[3] && valid[4] && valid[5] && valid[6] && valid[7] &&
        positions[3] == 0 && positions[4] == color && positions[5] == color &&
        positions[6] == color && positions[7] == color)
    {
        // 检查右侧是否被阻挡
        if (!valid[8] || positions[8] != 0)
        {
            rush_count++;
        }
    }

    // 修复跳冲四模式检测
    if (valid[0] && valid[1] && valid[2] && valid[3] && valid[4] &&
        positions[0] == color && positions[1] == color && positions[2] == color &&
        positions[3] == 0 && positions[4] == color)
    {
        // 检查是否被阻挡
        if (!valid[5] || positions[5] != 0)
        {
            rush_count++;
        }
    }

    return rush_count;
}


int long_link(int chess_board[], int size, int x, int y) // 长连检测，如是，返回1，否则返回0
{
    int pos = x * size + y;
    int color = chess_board[pos];

    // 若当前点为空或为白棋，直接返回0（白棋无长连禁手）
    if (color != 1)
        return 0;

    // 四个方向：水平、垂直、正斜线、反斜线
    int dirs[4][2] = {
        {1, 0}, // 水平
        {0, 1}, // 垂直
        {1, 1}, // 正斜线（左上到右下）
        {1, -1} // 反斜线（右上到左下）
    };

    for (int d = 0; d < 4; ++d)
    {
        int dx = dirs[d][0];
        int dy = dirs[d][1];

        int count = 1; // 当前点本身计数为1

        // 向一个方向延伸
        int nx = x + dx, ny = y + dy;
        while (nx >= 0 && nx < size && ny >= 0 && ny < size &&
               chess_board[nx * size + ny] == color)
        {
            count++;
            nx += dx;
            ny += dy;
        }

        // 向相反方向延伸
        nx = x - dx;
        ny = y - dy;
        while (nx >= 0 && nx < size && ny >= 0 && ny < size &&
               chess_board[nx * size + ny] == color)
        {
            count++;
            nx -= dx;
            ny -= dy;
        }

        // 若连续棋子数≥6，返回1（长连）
        if (count >= 6)
        {
            return 1;
        }
    }

    return 0; // 无长连
}

int check_win(int chess_board[], int size, int x, int y) // 检测五连，如是，返回1，否则返回0
{
    int pos = x * size + y;
    int color = chess_board[pos];

    // 如果当前位置为空，直接返回0
    if (color == 0)
        return 0;

    // 四个方向：水平、垂直、正斜线、反斜线
    int dirs[4][2] = {
        {1, 0}, // 水平
        {0, 1}, // 垂直
        {1, 1}, // 正斜线（左上到右下）
        {1, -1} // 反斜线（右上到左下）
    };

    for (int d = 0; d < 4; d++)
    {
        int dx = dirs[d][0];
        int dy = dirs[d][1];

        int count = 1; // 当前点本身计数为1

        // 向一个方向延伸
        int nx = x + dx, ny = y + dy;
        while (nx >= 0 && nx < size && ny >= 0 && ny < size &&
               chess_board[nx * size + ny] == color)
        {
            count++;
            nx += dx;
            ny += dy;
        }

        // 向相反方向延伸
        nx = x - dx;
        ny = y - dy;
        while (nx >= 0 && nx < size && ny >= 0 && ny < size &&
               chess_board[nx * size + ny] == color)
        {
            count++;
            nx -= dx;
            ny -= dy;
        }

        // 如果连续棋子数达到5个，返回1（获胜）
        if (count >= 5)
        {
            return 1;
        }
    }

    return 0; // 无五连
}

void record_move(int log[], int x, int y) // 仅记录棋步，不保存文件
{
    if (9 + log[4] < 400)
        log[9 + log[4]] = x << 7 | y;
    else
    {
        log[8] = -404;
        return;
    }
    log[4]++;
}

void save_final_log(int log[]) // 游戏结束时一次性保存日志文件
{
    FILE *fp = fopen("chess_board.log", "w");
    for (int i = 0; i < 400; i++)
    {
        fprintf(fp, "%d ", log[i]);
    }
    fclose(fp);
    int count = 1;
    while (1)
    {
        char filename[20];
        sprintf(filename, "%d.log", count);
        // 使用fopen检查文件是否存在，替代access函数
        FILE *test_fp = fopen(filename, "r");
        if (test_fp == NULL)
        {
            // 文件不存在，可以使用这个文件名
            rename("chess_board.log", filename);
            break;
        }
        else
        {
            // 文件存在，关闭文件句柄并尝试下一个文件名
            fclose(test_fp);
        }
        count++;
    }
}
