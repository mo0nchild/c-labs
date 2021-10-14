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

#define HORIZ 1
#define VERTIC 7

#define LIFE_COUNT 3

const char line[] = "\n -----\t -----\t -----\t -----\t -----\t -----\n";

enum KEY_CODE
{
    KEY_A = 97,
    KEY_D = 100,
    KEY_S = 115,
    KEY_W = 119,
    KEY_SPACE = 32
};

typedef enum state {WIN = 1, LOSE = 2, RUNNING = 0} GAME_STATE;

typedef struct CELL
{
    int free_cells_x, free_cells_y;
    int check_value;
} CELL;

typedef struct POSITION { int x, y; } POS;

POS set_POS(int x, int y) // êîíñòðóêòîð äëÿ ñòðóêòóðû POSITION
{
    POS pos = { .x = x, .y = y };
    return pos;
}

CELL field[7][7] = 
{
    { {7,7,0},{7,7,8},{7,7,0},{7,7,0},{7,7,0},{7,7,0},{7,7,9} },
    { {7,7,5},{7,7,3},{7,7,0},{7,7,0},{7,7,0},{7,7,3},{7,7,4} },
    { {7,7,0},{7,7,0},{7,7,0},{7,7,0},{7,7,4},{7,7,0},{7,7,0} },
    { {7,7,0},{7,7,4},{7,7,0},{7,7,0},{7,7,0},{7,7,6},{7,7,0} },
    { {7,7,0},{7,7,0},{7,7,5},{7,7,0},{7,7,0},{7,7,0},{7,7,0} },
    { {7,7,8},{7,7,9},{7,7,0},{7,7,0},{7,7,0},{7,7,10},{7,7,9} },
    { {7,7,2},{7,7,0},{7,7,0},{7,7,0},{7,7,0},{7,7,5},{7,7,0} }
};

typedef _Bool action_method (POS* p, HANDLE* c, int *l, _Bool* e);

void set_line(int dir, int* last, int i, int* cell);
_Bool get_keyboard_input(POS* pos);
_Bool check_axis(POS pos);
GAME_STATE draw_field(POS pos, HANDLE* c);

void run_app(void)
{
    int life_counter = LIFE_COUNT;
    _Bool game_end = FALSE;

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    POS cursor_pos = set_POS(0, 0);
start:
    for (int i = 0; i < 7; i++)
    {
        for (int k = 0; k < 7; k++) 
        {
            field[i][k].free_cells_x = 7;
            field[i][k].free_cells_y = 7;
            field[i][k].check_value = (field[i][k].check_value == BLACK_CELL ?
                WHITE_CELL : field[i][k].check_value);
        }
    }

    while (game_end != TRUE)
    {
        system("cls");
        printf("\nWASD: to control\tSPACE: to accept\nLIFE: %d\n", life_counter);

        switch (draw_field(cursor_pos, console))
        {

        case WIN: game_end = TRUE; 
        case LOSE:
            if (--life_counter <= 0) game_end = TRUE;
            check_axis(cursor_pos);
            continue;
        case RUNNING:
        default: break;
        }

        while (1) if (get_keyboard_input(&cursor_pos)) break;
    }

    system("cls");
    printf("\n\n\n\n\n\t\t%10s\n\n\n\n", (life_counter ?
        "YOU WIN" : "YOU LOSE"));

    Sleep(2000);
    life_counter = LIFE_COUNT;
    game_end = FALSE;
    goto start;
    

}

_Bool get_keyboard_input(POS *pos) 
{
    _Bool state = TRUE;
    switch (getch())
    {
    case KEY_A: pos->x = (pos->x - 1 < 0 ? 0 : pos->x - 1); break;
    case KEY_D: pos->x = (pos->x + 1 > 6 ? 6 : pos->x + 1); break;
    case KEY_W: pos->y = (pos->y - 1 < 0 ? 0 : pos->y - 1); break;
    case KEY_S: pos->y = (pos->y + 1 > 6 ? 6 : pos->y + 1); break;
    case KEY_SPACE: check_axis(*pos); break;
    default: state = FALSE; break;
    }
    return state;
}


void set_line(int dir, int* last, int i, int * cell)// ïåðåïèñü çíà÷åíèé âûáðàííîé îñè
{
    int value = i - (*last);
    for (; (*last) < i; (*last)++)
    {
        *cell = value;
        cell += (3 * dir);
    }
    (*last)++;
}

_Bool check_axis(POS pos)//ïðîâåðêà ïî äâóì îñÿì íà ïåðåñå÷åíèè x and y
{
    if (field[pos.y][pos.x].check_value > 0) return;
    field[pos.y][pos.x].check_value = (field[pos.y][pos.x].check_value == BLACK_CELL ?
        WHITE_CELL : BLACK_CELL);

    POS last = set_POS(0,0), cell = set_POS(0,0);
    for (int i = 0; i <= 7; i++)
    {
        if (field[pos.y][i].check_value == BLACK_CELL || i == 7)
            set_line(HORIZ, &last.x, i, &field[pos.y][last.x].free_cells_x);
        if (field[i][pos.x].check_value == BLACK_CELL || i == 7)
            set_line(VERTIC, &last.y, i, &field[last.y][pos.x].free_cells_y);
    }
    return TRUE;

}

GAME_STATE draw_field(POS pos, HANDLE *c)
{
    _Bool trigger = TRUE;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("%s", line);

    for (int y = 0; y < 7; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            if (pos.x == x && pos.y == y) SetConsoleTextAttribute(console, 112);

            if (field[y][x].check_value > 0) 
            {
                if (field[y][x].free_cells_x +
                    field[y][x].free_cells_y < field[y][x].check_value + 1)
                    return LOSE;

                else if (field[y][x].free_cells_x + field[y][x].free_cells_y 
                    != field[y][x].check_value + 1) 
                    trigger = FALSE; 

                printf("|%5d|", field[y][x].check_value);
            }
            else printf("|%5c|", field[y][x].check_value == WHITE_CELL ? ' ' : 'X');
            SetConsoleTextAttribute(console, 7);


        }
        printf("%s", line);
    }
    
    if (trigger) return WIN;
    return RUNNING;
}
