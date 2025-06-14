#include "chess_game.h"
//#include "print_win.h"
#include <stdio.h>
#include <windows.h>



int main()
{
    SetConsoleOutputCP(65001);
    int size = 19; // 棋盘大小
    int join = 1;  // 0表示人人对战，1表示人先走的人机对战，2表示机器先走的人机对战
    int win=chess_game(size,join);
    if(win==0)
    {
        printf("平局\n");
    }
    else if(win==1)
    {
        printf("黑棋胜利\n");
    }
    else if(win==-1)
    {
        printf("白棋胜利\n");
    }
    else
        printf("error");
    //print_win(win);
}
