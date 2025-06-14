#include "machine.h"
#include "chess_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// AI评分函数 - 评估某个位置的价值
int evaluate_position(int chess_board[], int size, int x, int y, int color)
{
    if (chess_board[x * size + y] != 0)
    {
        return -1000; // 位置已被占用
    }

    int score = 0;
    int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    // 临时放置棋子进行评估
    chess_board[x * size + y] = color;

    // 检查是否能形成五连（最高优先级）
    if (check_win(chess_board, size, x, y))
    {
        chess_board[x * size + y] = 0;
        return 10000;
    }

    // 检查是否能阻止对手形成五连
    chess_board[x * size + y] = -color;
    if (check_win(chess_board, size, x, y))
    {
        chess_board[x * size + y] = 0;
        return 5000;
    }
    chess_board[x * size + y] = color;

    // 评估四个方向的连子情况
    for (int d = 0; d < 4; d++)
    {
        int dx = dirs[d][0];
        int dy = dirs[d][1];
        int count = 1;
        int empty_ends = 0;

        // 向正方向计数
        int nx = x + dx, ny = y + dy;
        while (nx >= 0 && nx < size && ny >= 0 && ny < size)
        {
            if (chess_board[nx * size + ny] == color)
            {
                count++;
                nx += dx;
                ny += dy;
            }
            else if (chess_board[nx * size + ny] == 0)
            {
                empty_ends++;
                break;
            }
            else
            {
                break;
            }
        }

        // 向负方向计数
        nx = x - dx;
        ny = y - dy;
        while (nx >= 0 && nx < size && ny >= 0 && ny < size)
        {
            if (chess_board[nx * size + ny] == color)
            {
                count++;
                nx -= dx;
                ny -= dy;
            }
            else if (chess_board[nx * size + ny] == 0)
            {
                empty_ends++;
                break;
            }
            else
            {
                break;
            }
        }

        // 根据连子数和开放端数计算分数
        if (count >= 4)
            score += 1000;
        else if (count == 3 && empty_ends == 2)
            score += 500;
        else if (count == 3 && empty_ends == 1)
            score += 100;
        else if (count == 2 && empty_ends == 2)
            score += 50;
        else if (count == 2 && empty_ends == 1)
            score += 10;
    }

    chess_board[x * size + y] = 0; // 恢复原状态
    return score;
}

// 寻找最佳落子位置
void find_best_move(int chess_board[], int size, int *best_x, int *best_y, int color, int check_forbidden)
{
    int max_score = -10000;
    *best_x = size / 2;
    *best_y = size / 2;

    // 如果棋盘中心为空，优先选择中心位置
    if (chess_board[*best_x * size + *best_y] == 0)
    {
        return;
    }

    // 遍历棋盘寻找最佳位置
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            if (chess_board[x * size + y] != 0)
            {
                continue; // 位置已被占用
            }

            // 只考虑有棋子邻近的位置（提高效率）
            int has_neighbor = 0;
            for (int dx = -2; dx <= 2 && !has_neighbor; dx++)
            {
                for (int dy = -2; dy <= 2 && !has_neighbor; dy++)
                {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < size && ny >= 0 && ny < size)
                    {
                        if (chess_board[nx * size + ny] != 0)
                        {
                            has_neighbor = 1;
                        }
                    }
                }
            }

            if (!has_neighbor)
                continue;

            // 如果需要检查禁手（黑棋且log[1]=2）
            if (check_forbidden && color == 1)
            {
                chess_board[x * size + y] = color;
                if (check(chess_board, size, x, y))
                {
                    chess_board[x * size + y] = 0;
                    continue; // 跳过禁手位置
                }
                chess_board[x * size + y] = 0;
            }

            int score = evaluate_position(chess_board, size, x, y, color);
            if (score > max_score)
            {
                max_score = score;
                *best_x = x;
                *best_y = y;
            }
        }
    }
}

void click_machine(int chess_board[], int chess_board_size, int *log)
{
    static int initialized = 0;
    if (!initialized)
    {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }

    int ai_color;
    int check_forbidden = 0;

    // 根据log[1]确定AI颜色和是否检查禁手
    if (log[1] == 1)
    {
        // 人先走的人机对战，AI执行白棋，无需考虑禁手
        ai_color = -1;
        check_forbidden = 0;
    }
    else if (log[1] == 2)
    {
        // 机器先走的人机对战，AI执行黑棋，需要考虑禁手
        ai_color = 1;
        check_forbidden = 1;
    }
    else
    {
        // 异常情况，默认为白棋
        ai_color = -1;
        check_forbidden = 0;
    }

    int best_x, best_y;
    find_best_move(chess_board, chess_board_size, &best_x, &best_y, ai_color, check_forbidden);

    // 执行落子
    chess_board[best_x * chess_board_size + best_y] = ai_color;
    record_move(log, best_x, best_y); // 直接调用record_move
}
