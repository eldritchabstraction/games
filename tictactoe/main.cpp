#include <stdio.h>
#include <ctime>
#include <cstdlib>


int g_board[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int check_horizontal_win(int start)
{
    if (g_board[start])
    {
        if ((g_board[start] == g_board[start + 1]) && (g_board[start] == g_board[start + 2]))
        {
            printf("horizontal win from %d\n", start);
            return 1;
        }
    }
    return 0;
}

int check_vertical_win(int start)
{
    if (g_board[start])
    {
        if ((g_board[start] == g_board[start + 3]) && (g_board[start] == g_board[start + 6]))
        {
            printf("vertical win from %d\n", start);
            return 1;
        }
    }
    return 0;
}

int check_diagonal_win()
{
    if (g_board[1])
    {
        if ((g_board[1] == g_board[5]) && (g_board[1] == g_board[9]))
        {
            printf("diagonal win from 1\n");
            return 1;
        }
    }
    else if (g_board[3])
    {
        if ((g_board[3] == g_board[5]) && (g_board[3] == g_board[7]))
        {
            printf("diagonal win from 3\n");
            return 1;
        }
    }
    return 0;
}

int check_win_conditions()
{
    if (check_horizontal_win(1) || check_horizontal_win(4) || check_horizontal_win(7) ||
        check_vertical_win(1) || check_vertical_win(2) || check_vertical_win(3) || check_diagonal_win())
    {
        return 1;
    }
    return 0;
}

int main (void)
{
    std::srand(std::time(0));

    int player = 1;
    int turn = 0;
    while (turn < 9)
    {
        // Check win conditions
        if (check_win_conditions())
        {
            if (player == 1)
            {
                printf("player 2 wins\n");
            }
            else
            {
                printf("player 1 wins\n");
            }
                        
            return 0;
        }
        // printf("turn %d| player %d\n", turn, player);
        while (1)
        {
            int move = std::rand() % 10;
            // At this moment, we're going to not consider 0
            if (move == 0)
            {
                continue;
            }

            if (g_board[move] != 1 && g_board[move] != 2)
            {
                g_board[move] = player;
                if (player == 2)
                {
                    printf("    ");
                }
                printf("player %d takes %d\n", player, move);
                break;
            }
        }

        (player == 1) ? player = 2 : player = 1;

        turn++;
    }
    printf("gameover, no winner\n");
}
