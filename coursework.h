#pragma once//ver 1.2
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <malloc.h>

#include <dirent.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

#define WHITE_CELL 0
#define BLACK_CELL -1

#define CLEAR_FRAME(void) system("cls")

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
typedef enum { FALSE_T, TRUE_T, ACCEPT, BACK } ACTIONS;
typedef enum { CONTINUE, RETURN_TO_BEGIN, EXIT } UPDATE_ACTION;

typedef struct 
{
    UPDATE_ACTION state;
    void* return_value;
} UPDATE_T;
UPDATE_T set_update(UPDATE_ACTION state, _Bool return_value) 
{
    UPDATE_T update = { state, return_value };
    return update;
}

typedef char STR_NAME[261];

typedef struct
{
    int x;
    int y;
} POSITION;
POSITION set_position(int x, int y) // êîíñòðóêòîð äëÿ ñòðóêòóðû POSITION
{
    POSITION pos = { .x = x, .y = y };
    return pos;
}

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
    int field_size;
    char* filename;
    CELL* field;
    POSITION *current_cell;
    STR_NAME* dir;
    int dir_size;
    int life_counter;
} SETTING_PARAM;

void set_line(int dir, int* last, int i, int* cell);
void file_data(SETTING_PARAM* param, _Bool readonly);
void print_list(int cursor, int size, STR_NAME* items, int begin, int end);
void* update_frame(UPDATE_ACTION(*action)(SETTING_PARAM*, ACTIONS),
	SETTING_PARAM* param, POSITION max);
void save_file(SETTING_PARAM* param);
void print_rules(SETTING_PARAM* param);
void read_files(SETTING_PARAM* param);
void start_app(void);

UPDATE_ACTION dialog_box(SETTING_PARAM* param, ACTIONS a, POSITION pos);
UPDATE_ACTION game_loop(SETTING_PARAM* param, ACTIONS a, POSITION pos);

UPDATE_ACTION set_cell_value(SETTING_PARAM* param, ACTIONS a, POSITION pos);
UPDATE_ACTION set_cell_value(SETTING_PARAM* param, ACTIONS a, POSITION pos);
UPDATE_ACTION set_field_size(SETTING_PARAM* param, ACTIONS a, POSITION pos);
UPDATE_ACTION set_field_values(SETTING_PARAM* param, ACTIONS a, POSITION pos);

UPDATE_ACTION load_file(SETTING_PARAM* param, ACTIONS a, POSITION pos);
UPDATE_ACTION settings(SETTING_PARAM* param, ACTIONS a, POSITION pos);
UPDATE_ACTION mainmenu(SETTING_PARAM* param, ACTIONS a, POSITION pos);

GAME_STATE draw_field(POSITION pos, SETTING_PARAM* param);
ACTIONS get_keyboard_input(POSITION* pos, POSITION max);
_Bool check_axis(POSITION pos, SETTING_PARAM* param);

void* update_frame(UPDATE_ACTION(*action)(SETTING_PARAM*, ACTIONS),
	SETTING_PARAM* param, POSITION max)
{
	ACTIONS input = TRUE_T;
	POSITION pos = set_position(0, 0);
	UPDATE_ACTION update;

	while (1)
	{
		switch (input)
		{
		case BACK: return;
		case ACCEPT:
		case TRUE_T:
			CLEAR_FRAME();
			printf("\n[ W A S D: to control ] [ SPACE: to accept ] [ ESCAPE: to go back ]\n");
			update = action(param, input, pos);
			if (update || input == ACCEPT)
			{
				input = TRUE_T;
				if (update == EXIT) return;
				else continue;
			}
		case FALSE_T: break;
		}
		input = get_keyboard_input(&pos, max);
	}
}

void print_list(int cursor, int size, STR_NAME* items, int begin, int end)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\n\n\n");
	for (int i = begin; i < end; i++)
	{
		if (cursor == i)SetConsoleTextAttribute(console, 112);
		printf("\n\t\t\t[ %20.20s ]\n", i < size ? *(items + i) : "null");
		SetConsoleTextAttribute(console, 7);
	}
}


UPDATE_ACTION dialog_box(SETTING_PARAM* param, ACTIONS a, POSITION pos)
{
	if (a == ACCEPT)
	{
		switch (pos.x)
		{
		case 0: return EXIT;
		case 1: return;
		}
	}
	else
	{
		printf("\n\n\t\tARE YOU SURE?\n\n");
		const STR_NAME items[] = { "NO", "YES" };
		print_list(pos.x, 2, &items, 0, 2);
	}
	return CONTINUE;
}

UPDATE_ACTION game_loop(SETTING_PARAM* param, ACTIONS a, POSITION pos)
{
	printf("\n[ LIFE: %3d ] %d\n", param->life_counter, sizeof(param->field) / sizeof(CELL));
	if (a == ACCEPT) check_axis(pos, param);
	else
	{
		_Bool trigger = FALSE;
		switch (draw_field(pos, param))
		{
		case WIN: trigger = TRUE;
		case LOSE:
			if (--param->life_counter <= 0 || trigger == TRUE)
			{
				CLEAR_FRAME();
				printf("\n\n\n\t\tYOU %s\n", param->life_counter <= 0 ? "LOSE" : "WIN");
				getch();
				return EXIT;
			}
			check_axis(pos, param);
			return RETURN_TO_BEGIN;
		case RUNNING:break;
		}
	}
	return CONTINUE;
}

UPDATE_ACTION set_cell_value(SETTING_PARAM* param, ACTIONS a, POSITION pos)
{
	ACTIONS input = TRUE_T;
	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);

	if (a == ACCEPT)
	{
		(param->field + (param->current_cell->y * param->field_size + param->current_cell->x))
			->check_value = pos.x + 1;
		return EXIT;
	}
	else
	{
		printf(
			"\n\n\n\n\tset cell: %c %2d %c",
			(pos.x == 0) ? ' ' : '<',
			pos.x + 1,
			(pos.x == param->field_size * 2 - 2) ? ' ' : '>'
		);
	}
	return CONTINUE;
}

void save_file(SETTING_PARAM* param)
{
	CLEAR_FRAME();

	printf("\n\n\t\tfile name: ");
	scanf("%s", param->filename);

	strcat(param->filename, ".txt");
	file_data(param, FALSE);
}

UPDATE_ACTION set_field_size(SETTING_PARAM* param, ACTIONS a, POSITION pos)
{
	if (a == ACCEPT)
	{
		param->field_size = pos.x + 4;
		param->field = realloc(param->field, sizeof(CELL) * pow(pos.x + 4, 2));
		for (int i = 0; i < param->field_size; i++)
		{
			for (int k = 0; k < param->field_size; k++)
				*(param->field + (i * param->field_size + k)) = set_cell(0, param->field_size);
		}
		return EXIT;
	}
	else
	{
		printf(
			"\n\n\n\n\tset field size: %c %2d %c",
			(pos.x == 0) ? ' ' : '<',
			pos.x + 4,
			(pos.x == 6) ? ' ' : '>'
		);
	}
	return CONTINUE;
}


UPDATE_ACTION set_field_values(SETTING_PARAM* param, ACTIONS a, POSITION pos)
{
	if (a == ACCEPT)
	{
		*(param->current_cell) = pos;
		update_frame(set_cell_value, param, set_position(param->field_size * 2 - 1, 0));
	}
	else draw_field(pos, param);

	return CONTINUE;
}

void print_rules(SETTING_PARAM* param)
{
	CLEAR_FRAME();
	printf("current field: %s\n\n", param->filename);
	printf("\tÊóðîìàñó èãðàåòñÿ íà ïðÿìîóãîëüíîé ñåòêå. \nÂ íåêîòîðûõ èç ýòèõ ÿ÷ååê åñòü ÷èñëà. \nÊàæäàÿ ÿ÷åéêà ìîæåò áûòü ÷åðíîé èëè áåëîé. \nÖåëü ñîñòîèò â òîì, ÷òîáû îïðåäåëèòü, \nê êàêîìó òèïó îòíîñèòñÿ êàæäàÿ ÿ÷åéêà.\n\tÑëåäóþùèå ïðàâèëà îïðåäåëÿþò, êàêèå ÿ÷åéêè êàêèå :\n\t1)Êàæäîå ÷èñëî íà äîñêå ïðåäñòàâëÿåò êîëè÷åñòâî áåëûõ êëåòîê, \nêîòîðûå ìîæíî óâèäåòü èç ýòîé êëåòêè,\n âêëþ÷àÿ åå ñàìîãî.ß÷åéêó ìîæíî óâèäåòü èç äðóãîé ÿ÷åéêè,\n åñëè îíè íàõîäÿòñÿ â òîé æå ñòðîêå èëè ñòîëáöå,\n è ìåæäó íèìè íåò ÷åðíûõ ÿ÷ååê â ýòîé ñòðîêå èëè ñòîëáöå.\n\t2)Ïðîíóìåðîâàííûå ÿ÷åéêè íå ìîãóò áûòü ÷åðíûìè.\n\t3)Íèêàêèå äâå ÷åðíûå êëåòêè íå ìîãóò áûòü ñìåæíûìè ïî ãîðèçîíòàëè èëè âåðòèêàëè.\n\t4)Âñå áåëûå êëåòêè äîëæíû áûòü ñîåäèíåíû ãîðèçîíòàëüíî èëè âåðòèêàëüíî.\n");
	getch();
}

void read_files(SETTING_PARAM* param)
{
	DIR* d = opendir(".\\data");
	struct dirent* _dir;
	param->dir_size = 0;
	param->dir = (STR_NAME*)calloc(1, sizeof(STR_NAME));

	if (d)
	{
		while ((_dir = readdir(d)) != NULL && param->dir_size < 20)
		{
			param->dir = (STR_NAME*)realloc(param->dir, sizeof(STR_NAME) * (param->dir_size + 1));
			strcpy((param->dir + param->dir_size++), _dir->d_name);
		}
		closedir(d);
		param->dir_size -= 2;
		param->dir += 2;
	}
}


UPDATE_ACTION load_file(SETTING_PARAM* param, ACTIONS a, POSITION pos)
{
	pos.y = pos.y + pos.x * 3;
	if (a == ACCEPT && pos.y < param->dir_size)
	{
		param->filename = (char*)realloc(param->filename, sizeof(char) * 261);
		strcpy(param->filename, param->dir + pos.y);
		return EXIT;
	}
	else
	{
		print_list(pos.y, param->dir_size, param->dir, 3 * pos.x, 3 * (pos.x + 1));
		printf("\n\t\t\t%d/%d", pos.x + 1, (int)ceil(param->dir_size / 3.f));
	}
	return CONTINUE;
}

UPDATE_ACTION settings(SETTING_PARAM* param, ACTIONS a, POSITION pos)
{
	if (a == ACCEPT)
	{
		switch (pos.y)
		{
		case 0:
			update_frame(set_field_size, param, set_position(7, 0));
			update_frame(set_field_values, param, set_position(param->field_size, param->field_size));
			save_file(param); break;
		case 1:
			read_files(param);
			update_frame(load_file, param, set_position(ceil(param->dir_size / 3.f), 3)); break;
		case 2: print_rules(param);  break;
		}
	}
	else
	{
		const STR_NAME items[] = { "edit\0", "load\0", "rules\0" };
		print_list(pos.y, 3, items, 0, 3);
	}
	return CONTINUE;
}

UPDATE_ACTION mainmenu(SETTING_PARAM* param, ACTIONS a, POSITION pos)
{
	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
	if (a == ACCEPT)
	{
		switch (pos.y)
		{
		case 0: 
			file_data(param, TRUE);
			param->life_counter = LIFE_COUNT;
			update_frame(game_loop, param, set_position(7, 7));
			break;
		case 1: update_frame(settings, param, set_position(0, 3)); break;
		case 2: return EXIT;
		}
	}
	else
	{
		const STR_NAME items[] = { "start", "settings", "exit" };
		print_list(pos.y, 3, items, 0, 3);
	}

	return CONTINUE;

}

void start_app(void)
{
	system("mkdir data");

	SETTING_PARAM param =
	{
		.field_size = 7,
		.filename = (char*)calloc(261, sizeof(char)),
		.field = (CELL*)calloc(49, sizeof(CELL)),
		.current_cell = (POSITION*)calloc(1, sizeof(POSITION)),
	};
	strcpy(param.filename, "data.txt");
	file_data(&param, TRUE);

	for (int i = 0; i < param.field_size; i++)
	{
		for (int k = 0; k < param.field_size; k++)
			*(param.field + (i * param.field_size + k)) = set_cell(0, param.field_size);
	}
	update_frame(mainmenu, &param, set_position(0, 3));

}

void file_data(SETTING_PARAM* param, _Bool readonly)
{
    int index = 0;
    STR_NAME dirname;

    sprintf(dirname, "./data/%s", param->filename);
    FILE* file = fopen(dirname, readonly ? "rt" : "w");
    printf("%s\n", dirname);

    if (readonly) {
        fscanf(file, "%d", &(param->field_size));
        param->field = (CELL*)realloc(param->field, sizeof(CELL) * pow(param->field_size, 2));
        for (int i = 0; i < pow(param->field_size, 2); i++) 
        {
            (param->field + i)->free_cells_x = (param->field + i)->free_cells_y = param->field_size;
        }
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

ACTIONS get_keyboard_input(POSITION *pos, POSITION max) // ñòðåëî÷íîå óïðàâëåíèå WASD & SPACE
{
    ACTIONS state = TRUE_T;
    switch (getch())
    {
    case KEY_A: pos->x = (pos->x - 1 < 0 ? 0 : pos->x - 1); break;
    case KEY_D: 
        pos->x = (pos->x + 1 > max.x - 1 ? max.x - 1 : pos->x + 1);
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


void set_line(int dir, int* last, int i, int * cell) // ïåðåçàïèñü çíà÷åíèé ñâîáîíûõ êëåòîê âûáðàííîé îñè
{
    int value = i - (*last);
    for (; (*last) < i; (*last)++, cell += (3*dir)) *cell = value;
    (*last)++;
}

_Bool check_axis(POSITION pos, SETTING_PARAM* param) //ïðîâåðêà ïî äâóì îñÿì íà ïåðåñå÷åíèè x and y
{
    CELL *cell = (param->field + (pos.y * param->field_size + pos.x));

    if (cell->check_value > 0) return ;
    cell->check_value = (cell->check_value == BLACK_CELL ? WHITE_CELL : BLACK_CELL);

    POSITION last = set_position(0,0);
    for (int i = 0; i <= param->field_size; i++)
    {
        if ((param->field + (pos.y * param->field_size + i))->check_value == BLACK_CELL
            || i == param->field_size) 
        {
            set_line(1, &last.x, i, &((param->field + (pos.y * param->field_size + last.x))->free_cells_x));
        }
            
        if ((param->field + (i * param->field_size + pos.x))->check_value == BLACK_CELL
            || i == param->field_size) 
        {
            set_line(param->field_size, &last.y, i, &((param->field + (last.y * param->field_size + pos.x))->free_cells_y));
        }     
    }
    return TRUE;
}

GAME_STATE draw_field(POSITION pos, SETTING_PARAM *param )
{
    _Bool trigger = TRUE;
    GAME_STATE result = RUNNING;
    CELL* cell;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    
    printf("\n\n");
    for (int y = 0; y < param->field_size; y++)
    {
        for (int x = 0; x < param->field_size; x++)
        {
        cell = (param->field + (y * param->field_size + x));
        if (pos.x == x && pos.y == y) SetConsoleTextAttribute(console, 112);

        if (cell->check_value > 0)
        {
            if (cell->free_cells_x + cell->free_cells_y < cell->check_value + 1)
                result = LOSE;
            else if (cell->free_cells_x + cell->free_cells_y != cell->check_value + 1)
                trigger = FALSE;

            printf("|%5d|", cell->check_value);
        }
        else printf("|%5c|", cell->check_value == WHITE_CELL ? ' ' : 'X');
        SetConsoleTextAttribute(console, 7);
        }
        printf("\n\n");
    }   
    if (trigger) result = WIN;
    return result;
}
