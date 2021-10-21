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

#define CLEAR_FRAME(void) system("cls")
#define bool _Bool

#define LIFE_COUNT 3

const char FIELD_LINE[] = "\n -----\t -----\t -----\t -----\t -----\t -----\n";

typedef enum 
{
    KEY_A = 97,
    KEY_D = 100,
    KEY_S = 115,
    KEY_W = 119,
    KEY_SPACE = 32,
    KEY_ESC = 27
} KEY_CODE;
typedef enum { RUNNING, WIN, LOSE } GAME_STATE;
typedef enum { GAME, SETTINGS, EXIT, MAINMENU} SCENES;
typedef enum { FALSE_T, TRUE_T, ACCEPT, BACK } ACTIONS;

typedef struct
{
    int free_cells_x;
    int free_cells_y;
    int check_value;
} CELL;
CELL set_cell(int check, int free)
{
    CELL cell = { free, free, check };
    return cell;
}

typedef struct 
{ 
    int x; 
    int y;
} POSITION;
POSITION set_position(int x, int y) // конструктор для структуры POSITION
{
    POSITION pos = { .x = x, .y = y };
    return pos;
}

typedef struct
{
    int field_size;
    char* filename;
    SCENES scene;
    CELL* field;
} SETTING_PARAM;

CELL field[7][7];

void set_line(int dir, int* last, int i, int* cell);
void file_data(SETTING_PARAM* param, _Bool readonly);

ACTIONS get_keyboard_input(POSITION* pos, POSITION max);
bool check_axis(POSITION pos);

GAME_STATE draw_field(POSITION pos);



void run_app(SETTING_PARAM * param)
{
    POSITION cursor_pos = set_position(0, 0);
    file_data(param, TRUE);

    ACTIONS input = TRUE_T;
    for (int i = 0; i < param->field_size; i++)
    {
        for (int k = 0; k < param->field_size; k++) 
            field[i][k] = set_cell((param->field + (i * param->field_size + k))->check_value, param->field_size);
    }
    
    int life_counter = LIFE_COUNT;
    bool game_end = FALSE;

    while (game_end != TRUE)
    {
        switch (input)
        {
        case BACK: game_end = TRUE; life_counter = 0;  break;
        case ACCEPT: check_axis(cursor_pos);
        case TRUE_T:

            CLEAR_FRAME();
            printf("\nWASD: to control\tSPACE: to accept\nLIFE: %d\n", life_counter);

            switch (draw_field(cursor_pos, param))
            {

            case WIN: game_end = TRUE;
            case LOSE:
                if (--life_counter <= 0) game_end = TRUE;
                continue;
            case RUNNING:
            default: break;
            }
        case FALSE_T:break;
        }
        input = get_keyboard_input(&cursor_pos, set_position(param->field_size, param->field_size));
    }

    CLEAR_FRAME();
    printf("\n\n\n\n\n\t\t%10s\n\n\n\n", (life_counter ?
        "YOU WIN" : "YOU LOSE"));

    Sleep(2000);

    //goto start;

}

void file_data(SETTING_PARAM* param, _Bool readonly)
{
    int index = 0;
    char dirname[261];

    sprintf(dirname, "./data/%s", param->filename);
    FILE* file = fopen(dirname, readonly ? "rt" : "w");

    if (readonly) {
        fscanf(file, "%d", &(param->field_size));
        while (!feof(file))fscanf(file, "%d", &((param->field + (index++))->check_value));
    }
    else
    {
        fprintf(file, "%d\n", param->field_size);
        for (int i = 0; i < param->field_size; i++)
        {
            for (int k = 0; k < param->field_size; k++)
                fprintf(file, "%d\t", (param->field + (i * param->field_size + k))->check_value);
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

ACTIONS get_keyboard_input(POSITION *pos, POSITION max) // стрелочное управление WASD & SPACE
{
    ACTIONS state = TRUE_T;
    switch (getch())
    {
    case KEY_A: pos->x = (pos->x - 1 < 0 ? 0 : pos->x - 1); break;
    case KEY_D: 
        pos->x = (pos->x + 1 > max.x-1 ? max.x - 1 : pos->x + 1);
        break;
    case KEY_W: pos->y = (pos->y - 1 < 0 ? 0 : pos->y - 1); break;
    case KEY_S: 
        pos->y = (pos->y + 1 > max.y - 1 ? max.y - 1 : pos->y + 1);
        break;
    case KEY_SPACE: state = ACCEPT; break;
    case KEY_ESC: state = BACK; break;
    default: state = FALSE_T; break;
    }
    return state;
}


void set_line(int dir, int* last, int i, int * cell)// перезапись значений свобоных клеток выбранной оси
{
    int value = i - (*last);
    for (; (*last) < i; (*last)++, cell += (3 * dir)) *cell = value;
    (*last)++;
}

bool check_axis(POSITION pos)//проверка по двум осям на пересечении x and y
{
    if (field[pos.y][pos.x].check_value > 0) return ;
    field[pos.y][pos.x].check_value = (field[pos.y][pos.x].check_value 
        == BLACK_CELL ? WHITE_CELL : BLACK_CELL);

    POSITION last = set_position(0,0), cell = set_position(0,0);
    for (int i = 0; i <= 7; i++)
    {
        if (field[pos.y][i].check_value == BLACK_CELL || i == 7)
            set_line(HORIZ, &last.x, i, &field[pos.y][last.x].free_cells_x);
        if (field[i][pos.x].check_value == BLACK_CELL || i == 7)
            set_line(VERTIC, &last.y, i, &field[last.y][pos.x].free_cells_y);
    }
    return TRUE;
}




GAME_STATE draw_field(POSITION pos, SETTING_PARAM *param )
{
    _Bool trigger = TRUE;
    GAME_STATE result = RUNNING;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("%s", FIELD_LINE);

    for (int y = 0; y < param->field_size; y++)
    {
        for (int x = 0; x < param->field_size; x++)
        {
        if (pos.x == x && pos.y == y) SetConsoleTextAttribute(console, 112);

        if (field[y][x].check_value > 0)
        {
            if (field[y][x].free_cells_x +
                field[y][x].free_cells_y < field[y][x].check_value + 1)
                result = LOSE;

            else if (field[y][x].free_cells_x + field[y][x].free_cells_y
                != field[y][x].check_value + 1)
                trigger = FALSE;

            printf("|%5d|", field[y][x].check_value);
        }
        else printf("|%5c|", field[y][x].check_value == WHITE_CELL ? ' ' : 'X');
        SetConsoleTextAttribute(console, 7);


        }
        printf("%s", FIELD_LINE);
    }   

    if (trigger) result = WIN;
    return result;
}
