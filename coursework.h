#pragma once//ver 1.0
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <malloc.h>

#include <stdlib.h>
#include <Windows.h>
#include <string.h>

#define WHITE_CELL 0
#define BLACK_CELL -1

#define HORIZONTAL 1
#define VERTICAL 2

#define LIFE_COUNT 3

enum KEY_CODE
{
    KEY_A = 97,
    KEY_D = 100,
    KEY_S = 115,
    KEY_W = 119,
    KEY_SPACE = 32
};

struct CELL
{
    int free_cells_x, free_cells_y;
    int check_value;
}
field[7][7] =
{
    {{7,7,0},{7,7,8},{7,7,0},{7,7,0},{7,7,0},{7,7,0},{7,7,0}},
    {{7,7,5},{7,7,3},{7,7,0},{7,7,0},{7,7,0},{7,7,3},{7,7,4}},
    {{7,7,0},{7,7,0},{7,7,0},{7,7,0},{7,7,4},{7,7,0},{7,7,0}},
    {{7,7,0},{7,7,4},{7,7,0},{7,7,0},{7,7,0},{7,7,6},{7,7,0}},
    {{7,7,0},{7,7,0},{7,7,5},{7,7,0},{7,7,0},{7,7,0},{7,7,0}},
    {{7,7,8},{7,7,9},{7,7,0},{7,7,0},{7,7,0},{7,7,10},{7,7,9}},
    {{7,7,2},{7,7,0},{7,7,0},{7,7,0},{7,7,0},{7,7,5},{7,7,0}}
};

_Bool check_axis(int x, int y)
{
    int cell_y = 0, cell_x = 0, last_y = 0, last_x = 0;
    for (int i = 0; i <= 7; i++)
    {

        if (field[y][i].check_value == BLACK_CELL || i == 7)
        {
            cell_x = i - last_x;
            for (; last_x < i; last_x++) field[y][last_x].free_cells_x = cell_x;
            last_x++;
        }
        if (field[i][x].check_value == BLACK_CELL || i == 7)
        {
            cell_y = i - last_y;
            for (; last_y < i; last_y++) field[last_y][x].free_cells_y = cell_y;
            last_y++;
        }

    }
    return TRUE;

}

void check_field(int x, int y)
{
    if (field[y][x].check_value > 0) return;
    field[y][x].check_value = (field[y][x].check_value == BLACK_CELL ?
        WHITE_CELL : BLACK_CELL);
    /*field[y][x].free_cells_x = (field[y][x].free_cells_x == -1 ?
        7 : -1);
    field[y][x].free_cells_y = (field[y][x].free_cells_y == -1 ?
        7 : -1);*/

    check_axis(x, y);

}

_Bool draw_field(int pos_x, int pos_y, _Bool *trigger, 
    HANDLE * c, _Bool * click)
{
    if (*click)
    {
        check_field(pos_x, pos_y);
        *click = FALSE;
    }
    *trigger = TRUE;
    printf("\n");
    for (int i = 0; i < 50; i++) printf("-");
    printf("\n");
    for (int y = 0; y < 7; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            if (field[y][x].check_value > 0) 
            {
                if (field[y][x].free_cells_x +
                    field[y][x].free_cells_y < field[y][x].check_value + 1)
                {
                    check_field(pos_x, pos_y); 
                    return FALSE;
                }
                else if (field[y][x].free_cells_x +
                    field[y][x].free_cells_y != field[y][x].check_value + 1) 
                {
                    *trigger = FALSE;
                }
            }
            /* printf("  %d %d  ", field[y][x].free_cells_x + field[y][x].free_cells_y - 1
                 , field[y][x].check_value);*/
            if(pos_x == x && pos_y == y)SetConsoleTextAttribute(*c, 112);
            printf("|%5d|", field[y][x].check_value);
            SetConsoleTextAttribute(*c, 7);
            
        }
        printf("\n");
        for (int i = 0; i < 50; i++) printf("-");
        printf("\n");
    }


    return TRUE;
}

void run_app(void)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    int x = 0, y = 0, life_counter = LIFE_COUNT;
    _Bool win_check, click = FALSE;

    while (1)
    {
        system("cls");
        printf("\nWASD: to control\tSPACE: to accept\nLIFE: %d\n", life_counter);
        if (!draw_field(x, y, &win_check, &console, &click))
        {
            if (--life_counter <= 0) break;
            continue;
        }

        if (win_check == TRUE) break;
        /*scanf("%d %d", &x, &y);*/
        while (1) 
        {
            switch (getch())
            {
            case KEY_A: x = (x - 1 < 0 ? 0 : x - 1); break;
            case KEY_D: x = (x + 1 > 6 ? 6 : x + 1); break;
            case KEY_W: y = (y - 1 < 0 ? 0 : y - 1); break;
            case KEY_S: y = (y + 1 > 6 ? 6 : y + 1); break;
            case KEY_SPACE: click = TRUE; break;
            default: continue;
            }
            break;
        }
    }
    system("cls");
    printf("\n\n\n\n\n\t\tGAME END\n\n\n\n");

}