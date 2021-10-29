#pragma once//ver 1.5
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <malloc.h>

#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <dirent.h>

#define WHITE_CELL 0
#define BLACK_CELL -1
#define LIFE_COUNT 3
#define MAX_FILES_IN_DIR 20
#define clear_frame(void) system("cls")

typedef enum 
{
    key_a = 97,
    key_d = 100,
    key_s = 115,
    key_w = 119,
    key_space = 32,
    key_esc = 27
} KEY_CODE;
typedef enum { RUNNING, WIN, LOSE } gstate_t;
typedef enum { FALSE_T, TRUE_T, ACCEPT, BACK } gaction_t;
typedef enum { CONTINUE, RETURN_TO_BEGIN, EXIT } gupdate_t;

typedef char name_t[261];
typedef _Bool bool;

typedef struct{int x, y;} tuple_t;
tuple_t tuple_c(int x, int y) // конструктор дл€ структуры tuple_t
{
	tuple_t constructor = { x, y };
    return constructor;
}

typedef struct
{
	tuple_t free_value;
    int check_value;
} cell_t;
cell_t cell_c(int check, int free)
{
    cell_t constructor = { tuple_c(free, free), check };
    return constructor;
}

typedef struct 
{
	name_t* array;
	int size;
} dir_t;
dir_t dir_c(name_t* dir, int size) 
{
	dir_t constructor = {dir, size};
	return constructor;
}

typedef struct
{
    int field_size;
    char* filename;
    cell_t* field;
    tuple_t *current_cell;
	dir_t dir;
} gparam_t;

typedef gupdate_t (*update_action_function)(gparam_t* param, gaction_t a, tuple_t pos);

void set_line(int dir, int* last, int i, int* cell);
void file_data(gparam_t* param, bool readonly);
void print_list(int cursor, int size, name_t* items, int begin, int end);
void* update_frame(update_action_function action, gparam_t* param, tuple_t max);
void save_file(gparam_t* param);
void print_rules(gparam_t* param);
void read_files(gparam_t* param);
void start_app(void);

gupdate_t set_cell_value(gparam_t* param, gaction_t a, tuple_t pos);
gupdate_t set_field_size(gparam_t* param, gaction_t a, tuple_t pos);
gupdate_t set_field_values(gparam_t* param, gaction_t a, tuple_t pos);

gupdate_t load_file(gparam_t* param, gaction_t a, tuple_t pos);
gupdate_t mainmenu(gparam_t* param, gaction_t a, tuple_t pos);
gupdate_t settings(gparam_t* param, gaction_t a, tuple_t pos);
gupdate_t game_loop(gparam_t*, gaction_t, tuple_t);

gstate_t draw_field(tuple_t pos, gparam_t* param);
gaction_t get_keyboard_input(tuple_t* pos, tuple_t max);

bool check_axis(tuple_t pos, gparam_t* param);
bool dialog_box(const char* label);

void* update_frame(update_action_function action, gparam_t* param, tuple_t max)
{
	gaction_t input = TRUE_T;
	tuple_t pos = tuple_c(0, 0);

	while (1)
	{
		switch (input)
		{
		case BACK: return;
		case ACCEPT:
		case TRUE_T:
			clear_frame();
			printf("\n[ W A S D: to control ] [ SPACE: to accept ] [ ESCAPE: to go back ]\n");
			gupdate_t update = action(param, input, pos);
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

bool dialog_box(const char* label) 
{
	clear_frame();
	printf("\n[ESC: to return]   [ANY KEY: to accept]\n\n\n\n\n\t\t\t%s\n", label);
	return ((int)getch() == key_esc ? FALSE : TRUE);
}

void print_list(int cursor, int size, name_t* items, int begin, int end)
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

gupdate_t game_loop(gparam_t* param, gaction_t a, tuple_t pos)
{
	static int life_counter = LIFE_COUNT;
	char *life_line = (char*)calloc(life_counter*3 + 1, sizeof(char));
	for (int i = 0; i < life_counter; i++) strcat(life_line, "\x03  ");

	printf("\n[ LIFE: %8.8s ]\n", life_line);
	if (a == ACCEPT) check_axis(pos, param);
	else
	{
		bool trigger = FALSE;
		switch (draw_field(pos, param))
		{
		case WIN: trigger = TRUE;
		case LOSE:
			if (--life_counter <= 0 || trigger == TRUE)
			{
				clear_frame();
				printf("\n\n\n\t\tYOU %s\n", life_counter <= 0 ? "LOSE" : "WIN");
				getch();
				life_counter = LIFE_COUNT;
				return EXIT;
			}
			check_axis(pos, param);
			return RETURN_TO_BEGIN;
		case RUNNING:break;
		}
	}
	return CONTINUE;
}

gupdate_t set_cell_value(gparam_t* param, gaction_t a, tuple_t pos)
{
	if (a == ACCEPT)
	{
		(param->field + (param->current_cell->y * param->field_size + param->current_cell->x))
			->check_value = pos.x + 1;
		return EXIT;
	}
	else
	{
		printf("\n\n\n\n\tset cell: %c %2d %c", (pos.x == 0) ? ' ' : '<', pos.x + 1,
			(pos.x == param->field_size * 2 - 2) ? ' ' : '>');
	}
	return CONTINUE;
}

void save_file(gparam_t* param)
{
	clear_frame();

	printf("\n\n\t\tfile name: ");
	scanf("%s", param->filename);

	strcat(param->filename, ".txt");
	file_data(param, FALSE);
}

gupdate_t set_field_size(gparam_t* param, gaction_t a, tuple_t pos)
{
	if (a == ACCEPT)
	{
		param->field_size = pos.x + 4;
		param->field = realloc(param->field, sizeof(cell_t) * pow(pos.x + 4, 2));
		for (int i = 0; i < param->field_size; i++)
		{
			for (int k = 0; k < param->field_size; k++)
				*(param->field + (i * param->field_size + k)) = cell_c(0, param->field_size);
		}
		return EXIT;
	}
	else
	{
		printf("\n\n\n\n\tset field size: %c %2d %c",(pos.x == 0) ? ' ' : '<',
			pos.x + 4, (pos.x == 6) ? ' ' : '>');
	}
	return CONTINUE;
}


gupdate_t set_field_values(gparam_t* param, gaction_t a, tuple_t pos)
{
	if (a == ACCEPT)
	{
		*(param->current_cell) = pos;
		update_frame(set_cell_value, param, tuple_c(param->field_size * 2 - 1, 0));
	}
	else draw_field(pos, param);

	return CONTINUE;
}

void print_rules(gparam_t* param)
{
	clear_frame();
	printf("current field: %s\n\n", param->filename);
	printf("\t уромасу играетс€ на пр€моугольной сетке. \n¬ некоторых из этих €чеек есть числа. \n ажда€ €чейка может быть черной или белой. \n÷ель состоит в том, чтобы определить, \nк какому типу относитс€ кажда€ €чейка.\n\t—ледующие правила определ€ют, какие €чейки какие :\n\t1) аждое число на доске представл€ет количество белых клеток, \nкоторые можно увидеть из этой клетки,\n включа€ ее самого.ячейку можно увидеть из другой €чейки,\n если они наход€тс€ в той же строке или столбце,\n и между ними нет черных €чеек в этой строке или столбце.\n\t2)ѕронумерованные €чейки не могут быть черными.\n\t3)Ќикакие две черные клетки не могут быть смежными по горизонтали или вертикали.\n\t4)¬се белые клетки должны быть соединены горизонтально или вертикально.\n");
	getch();
}

void read_files(gparam_t* param)
{
	DIR* directory = opendir(".\\data");
	struct dirent* ptr_dirent_value;
	param->dir.size = 0;
	param->dir.array = (name_t*)calloc(1, sizeof(name_t));

	if (directory) 
	{
		while ((ptr_dirent_value = readdir(directory)) != NULL && param->dir.size < MAX_FILES_IN_DIR)
		{
			param->dir.array = (name_t*)realloc(param->dir.array, sizeof(name_t) * (param->dir.size + 1));
			strcpy((param->dir.array + param->dir.size++), ptr_dirent_value->d_name);
		}
		closedir(directory);
		param->dir.size -= 2;
		param->dir.array += 2;
	}

}


gupdate_t load_file(gparam_t* param, gaction_t a, tuple_t pos)
{
	pos.y = pos.y + pos.x * 3;

	if (a == ACCEPT && pos.y < param->dir.size)
	{
		param->filename = (char*)realloc(param->filename, sizeof(char) * 261);
		strcpy(param->filename, param->dir.array + pos.y);
		return EXIT;
	}
	else
	{
		print_list(pos.y, param->dir.size, param->dir.array, 3 * pos.x, 3 * (pos.x + 1));
		printf("\n\t\t\t%d/%d", pos.x + 1, (int)ceil(param->dir.size / 3.f));
	}
	return CONTINUE;
}

gupdate_t settings(gparam_t* param, gaction_t a, tuple_t pos)
{
	if (a == ACCEPT)
	{
		switch (pos.y)
		{
		case 0:
			update_frame(set_field_size, param, tuple_c(7, 0));
			update_frame(set_field_values, param, tuple_c(param->field_size, param->field_size));
			save_file(param); break;
		case 1:
			read_files(param);
			update_frame(load_file, param, tuple_c(ceil(param->dir.size / 3.f), 3)); break;
		case 2: print_rules(param);  break;
		}
	}
	else
	{
		const name_t items[] = { "edit\0", "load\0", "rules\0" };
		print_list(pos.y, 3, items, 0, 3);
	}
	return CONTINUE;
}

gupdate_t mainmenu(gparam_t* param, gaction_t a, tuple_t pos)
{
	if (a == ACCEPT)
	{
		switch (pos.y)
		{
		case 0:;
			bool trigger = FALSE;
			file_data(param, TRUE);
			while (!trigger) 
			{
				update_frame(game_loop, param, tuple_c(param->field_size, param->field_size));
				trigger = dialog_box("DO YOU WANT EXIT?");
			}
			break;
		case 1: update_frame(settings, param, tuple_c(0, 3)); break;
		case 2: return EXIT;
		}
	}
	else
	{
		const name_t items[] = { "start", "settings", "exit" };
		print_list(pos.y, 3, items, 0, 3);
	}

	return CONTINUE;

}

void start_app(void)
{
	setlocale(LC_ALL, "rus");
	system("mkdir data");

	gparam_t param =
	{
		.field_size = 7,
		.filename = (char*)calloc(261, sizeof(char)),
		.field = (cell_t*)calloc(49, sizeof(cell_t)),
		.current_cell = (tuple_t*)calloc(1, sizeof(tuple_t)),
	};
	strcpy(param.filename, "data.txt");
	file_data(&param, TRUE);

	for (int i = 0; i < param.field_size; i++)
	{
		for (int k = 0; k < param.field_size; k++)
			*(param.field + (i * param.field_size + k)) = cell_c(0, param.field_size);
	}
	update_frame(mainmenu, &param, tuple_c(0, 3));

}

void file_data(gparam_t* param, bool readonly)
{
    int index = 0;
    name_t dirname;

    sprintf(dirname, "./data/%s", param->filename);
    FILE* file = fopen(dirname, readonly ? "rt" : "w");
    printf("%s\n", dirname);

    if (readonly) {
        fscanf(file, "%d", &(param->field_size));
        param->field = (cell_t*)realloc(param->field, sizeof(cell_t) * pow(param->field_size, 2));
        for (int i = 0; i < pow(param->field_size, 2); i++) 
        {
            (param->field + i)->free_value.x = (param->field + i)->free_value.y = param->field_size;
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

gaction_t get_keyboard_input(tuple_t *pos, tuple_t max) // стрелочное управление WASD & SPACE
{
    gaction_t state = TRUE_T;
    switch (getch())
    {
    case key_a: pos->x = (pos->x - 1 < 0 ? 0 : pos->x - 1); break;
    case key_d: 
        pos->x = (pos->x + 1 > max.x - 1 ? max.x - 1 : pos->x + 1);
        break;
    case key_w: pos->y = (pos->y - 1 < 0 ? 0 : pos->y - 1); break;
    case key_s: 
        pos->y = (pos->y + 1 > max.y - 1 ? max.y - 1 : pos->y + 1);
        break;
    case key_space: state = ACCEPT; break;
    case key_esc: state = BACK; break;
    default: state = FALSE_T; break;
    }
    return state;
}


void set_line(int dir, int* last, int i, int * cell) // перезапись значений свобоных клеток выбранной оси
{
    int value = i - (*last);
    for (; (*last) < i; (*last)++, cell += (3*dir)) *cell = value;
    (*last)++;
}

bool check_axis(tuple_t pos, gparam_t* ptr_param) //проверка по двум ос€м на пересечении x and y
{
    cell_t *cell = (ptr_param->field + (pos.y * ptr_param->field_size + pos.x));

    if (cell->check_value > 0) return ;
    cell->check_value = (cell->check_value == BLACK_CELL ? WHITE_CELL : BLACK_CELL);

    tuple_t last = tuple_c(0,0);
    for (int i = 0; i <= ptr_param->field_size; i++)
    {
        if ((ptr_param->field + (pos.y * ptr_param->field_size + i))->check_value == BLACK_CELL
            || i == ptr_param->field_size)
        {
            set_line(1, &last.x, i, &((ptr_param->field + 
				(pos.y * ptr_param->field_size + last.x))->free_value.x));
        }
            
        if ((ptr_param->field + (i * ptr_param->field_size + pos.x))->check_value == BLACK_CELL
            || i == ptr_param->field_size)
        {
            set_line(ptr_param->field_size, &last.y, i, &((ptr_param->field + 
				(last.y * ptr_param->field_size + pos.x))->free_value.y));
        }     
    }
    return TRUE;
}

gstate_t draw_field(tuple_t pos, gparam_t *param )
{
    bool trigger = TRUE;
    gstate_t result = RUNNING;
    cell_t* cell;
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
            if (cell->free_value.x + cell->free_value.y < cell->check_value + 1)
                result = LOSE;
            else if (cell->free_value.x + cell->free_value.y != cell->check_value + 1)
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
