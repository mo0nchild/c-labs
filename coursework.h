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
typedef enum
{
	RED = 64,
	GREEN = 32,
	DEFAULT = 7,
	SELECT = 112
} color_t;
typedef enum { RUNNING, WIN, LOSE } gstate_t;
typedef enum { FALSE_T, TRUE_T, ACCEPT, BACK } gaction_t;
typedef enum { CONTINUE, RETURN_TO_BEGIN, EXIT } fupdate_t;

typedef char name_t[261];
typedef _Bool bool;

typedef struct 
{
	fupdate_t frame_update_state;
	void* return_value;
} gupdate_t;
gupdate_t gupdate_c(fupdate_t f, void* v) 
{
	gupdate_t constructor = { f, v };
	return constructor;
}

typedef struct{int x, y;} tuple_t;
tuple_t tuple_c(int x, int y) // конструктор для структуры tuple_t
{
	tuple_t constructor = { x, y };
	return constructor;
}

typedef struct
{
	tuple_t free_value;
	int check_value;
	color_t color;
} cell_t;
cell_t cell_c(int check, int free, color_t color)
{
	cell_t constructor = { tuple_c(free, free), check ,color };
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
	cell_t* array;
	name_t name;
	int size;
}field_t;
field_t field_c(cell_t* a, name_t n,int s) 
{
	field_t constructor = { a, n, s };
	return constructor;
}

typedef struct
{
	field_t field;
	tuple_t *current_cell;
	dir_t dir;
} gparam_t;

typedef gupdate_t (*update_action_function)(int param_size, void* param[], 
	gaction_t a, tuple_t pos);

void set_line(int dir, int* last, int i, int* cell);
void print_list(int cursor, int size, name_t* items, int begin, int end);

void* update_frame(update_action_function action, tuple_t max,
	bool back_use, void* param[]);

void print_rules(void);
void start_app(void);

gupdate_t set_cell_value(int size, void* args[], gaction_t a, tuple_t pos);
gupdate_t set_field_size(int size, void* args[],gaction_t a, tuple_t pos);
gupdate_t set_field_values(int size, void* args[], gaction_t a, tuple_t pos);
gupdate_t dialog_box(int size, void* args[], gaction_t a, tuple_t pos);

gupdate_t load_file(int size, void* args[], gaction_t a, tuple_t pos);
gupdate_t mainmenu(int size, void* args[], gaction_t a, tuple_t pos);
gupdate_t settings(int size, void* args[], gaction_t a, tuple_t pos);
gupdate_t game_loop(int size, void* args[], gaction_t a, tuple_t pos);

gstate_t draw_field(tuple_t pos, field_t* param);
gaction_t get_keyboard_input(tuple_t* pos, tuple_t max);

bool check_axis(tuple_t pos, gparam_t* param);
bool file_data(field_t* field, bool readonly);
bool read_files(dir_t* param);

void* update_frame(update_action_function action, tuple_t max, 
	bool back_use, void * param[])
{
	gaction_t input = TRUE_T;
	tuple_t pos = tuple_c(0, 0);

	while (1)
	{
		switch (input)
		{
		case BACK: if (!back_use) break;
		case ACCEPT: 
		case TRUE_T:
			clear_frame();
			printf("\n[ W A S D: to control ] [ SPACE: to accept ] [ %18.18s ]\n", 
				(back_use) ? "ESCAPE: to go back" : "==================");
			gupdate_t update = action(0, param, input, pos);
			if (update.frame_update_state || input == ACCEPT)
			{
				input = TRUE_T;
				if (update.frame_update_state == EXIT) return update.return_value;
				else continue;
			}
		case FALSE_T: break;
		}
		input = get_keyboard_input(&pos, max);
	}
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

gupdate_t set_cell_value(int size, void* args[], gaction_t a, tuple_t pos)
{
	int* field_size = (int*)args;
	if (a == ACCEPT) 
	{
		int value = pos.x;
		return gupdate_c(EXIT, &value);
	}
	else
	{
		printf("\n\n\n\n\t\t[ set cell: %c %2d %c ]", (pos.x == 0) ? ' ' : '<', pos.x,
			(pos.x == *field_size * 2 - 1) ? ' ' : '>');
	}
	return gupdate_c(CONTINUE, NULL);
}

gupdate_t set_field_size(int size, void* args[], gaction_t a, tuple_t pos)
{
	if (a == ACCEPT)
	{
		field_t field;

		field.size = pos.x + 4;
		field.array = (cell_t*)calloc(pow(pos.x + 4, 2), sizeof(cell_t));

		for (int i = 0; i < field.size; i++)
		{
			for (int k = 0; k < field.size; k++)
				*(field.array + (i * field.size + k)) = cell_c(0, field.size, DEFAULT);
		}//------------------------------------------------------------------------------------------------------------------------------
		return gupdate_c(EXIT, &field);
	}
	else
	{
		printf("\n\n\n\n\t\t[ set field size: %c %2d %c ]",(pos.x == 0) ? ' ' : '<',
			pos.x + 4, (pos.x == 6) ? ' ' : '>');
	}
	return gupdate_c(CONTINUE, NULL);
}

gupdate_t dialog_box(int size, void* args[], gaction_t a, tuple_t pos) 
{
	dir_t* items = (dir_t*)args;
	if (a == ACCEPT) 
	{
		int select = (pos.y);
		return gupdate_c(EXIT, &select);
	}

	printf("\n\n\n\t\t%s\n", "WHAT DO YOU WANT TO DO?");
	print_list(pos.y, items->size, items->array, 0, items->size);

	return gupdate_c(CONTINUE, NULL);
}

gupdate_t set_field_values(int size, void* args[], gaction_t a, tuple_t pos)
{
	field_t* field = (field_t*)args;
	if (a == ACCEPT)
	{
		int *value = (int*)update_frame(set_cell_value, tuple_c(field->size * 2, 0), FALSE, &(field->size));
		printf("%d\n", *value);
		(field->array + (pos.y * field->size + pos.x))->check_value = *value;
	}
	else if (a == BACK)
	{
		dir_t items = dir_c((name_t*)calloc(3, sizeof(name_t)), 3);
		strcpy(items.array, "continue");
		strcpy(items.array + 1, "exit");
		strcpy(items.array + 2, "save");
		switch (*(int*)update_frame(dialog_box, tuple_c(0, 3), FALSE, &items))
		{
		case 0: break;
		case 1: return gupdate_c(EXIT, NULL);
		case 2: 
			clear_frame();
			printf("\n\n\n\t\t\t[ input field name ]: ");
			scanf("%s", field->name);
			return gupdate_c(EXIT, field);
		}
	}
	draw_field(pos, field);
	return gupdate_c(CONTINUE, NULL);
}

void print_rules(void)
{
	clear_frame();
	printf("\tКуромасу играется на прямоугольной сетке. \nВ некоторых из этих ячеек есть числа. \nКаждая ячейка может быть черной или белой. \nЦель состоит в том, чтобы определить, \nк какому типу относится каждая ячейка.\n\tСледующие правила определяют, какие ячейки какие :\n\t1)Каждое число на доске представляет количество белых клеток, \nкоторые можно увидеть из этой клетки,\n включая ее самого.Ячейку можно увидеть из другой ячейки,\n если они находятся в той же строке или столбце,\n и между ними нет черных ячеек в этой строке или столбце.\n\t2)Пронумерованные ячейки не могут быть черными.\n\t3)Никакие две черные клетки не могут быть смежными по горизонтали или вертикали.\n\t4)Все белые клетки должны быть соединены горизонтально или вертикально.\n");
	getch();
}

bool read_files(dir_t * param)
{
	DIR* directory = opendir(".\\data");
	struct dirent* ptr_dirent_value;
	param->size = 0;
	param->array = (name_t*)calloc(1, sizeof(name_t));

	if (!directory) return FALSE;
	
	while ((ptr_dirent_value = readdir(directory)) != NULL && param->size < MAX_FILES_IN_DIR)
	{
		param->array = (name_t*)realloc(param->array, sizeof(name_t) * (param->size + 1));
		strcpy((param->array + param->size++), ptr_dirent_value->d_name);
	}
	closedir(directory);

	param->size -= 2;
	param->array += 2;

	return TRUE;
}

gupdate_t load_file(int size, void* args[], gaction_t a, tuple_t pos)
{
	pos.y = pos.y + pos.x * 3;
	dir_t *param = (dir_t*)args;
	if (a == ACCEPT && pos.y < param->size)
	{
		name_t filename;
		strcpy(filename, param->array + pos.y);
		return gupdate_c(EXIT, filename);
	}
	else if(a == BACK) return gupdate_c(EXIT, NULL);
	else
	{
		print_list(pos.y, param->size, param->array, 3 * pos.x, 3 * (pos.x + 1));
		printf("\n\t\t\tpage: %d/%d", pos.x + 1, (int)ceil(param->size / 3.f));
	}
	return gupdate_c(CONTINUE, NULL);
}

gupdate_t settings(int size, void* args[], gaction_t a, tuple_t pos)
{
	field_t* field = (field_t*)(args);
	if (a == ACCEPT)
	{
		switch (pos.y)
		{
		case 0:;
			field_t edit_field = *(field_t*)update_frame(set_field_size, tuple_c(7, 0), FALSE, NULL);
			field_t *save_field = (field_t*)update_frame(set_field_values, tuple_c(edit_field.size, edit_field.size), TRUE, &edit_field);

			if (save_field != NULL) 
			{
				if (!file_data(save_field, FALSE))
				{
					clear_frame();
					printf("\n\n\n\t\t\t[ can't save field ]");
				}
				else *field = *save_field;
			}
			break;
		case 1:;
			dir_t dir;
			if (read_files(&dir)) 
			{
				name_t* filename = (name_t*)update_frame(load_file, tuple_c(ceil(dir.size / 3.f), 3), TRUE, &dir);
				if(filename != NULL)strcpy(field->name, *filename);
			}
			break;
		case 2: print_rules();  break;
		case 3: return gupdate_c(EXIT, field);
		}
	}
	else if(a == BACK) return gupdate_c(EXIT, field);
	else 
	{
		printf("\ncurrent field: %s", field->name);

		const name_t items[] = { "edit", "load", "rules", "back" };
		print_list(pos.y, 4, items, 0, 4);
	}
	return gupdate_c(CONTINUE, NULL);
}

gupdate_t mainmenu(int size, void* args[], gaction_t a, tuple_t pos)
{
	gparam_t *param = (gparam_t*)args;

	if (a == ACCEPT)
	{
		switch (pos.y)
		{
		case 0:;
			bool trigger = FALSE;
			bool check = file_data(&(param->field), TRUE);
			update_frame(game_loop, tuple_c(param->field.size, param->field.size), TRUE, param);

			break;
		case 1:; 
			field_t *field = (field_t*)update_frame(settings, tuple_c(0, 4), FALSE, &param->field);
			param->field = *field;
			break;
		case 2: return gupdate_c(EXIT, NULL);
		}
	}
	else
	{
		printf("\n\t\t%s\n\t\t%s\n\t\t%s", 
			"| /  \\  /   |==|  |==|  |\\ /|    \\    |==|  \\  /", 
			"|\\     /    |__|  |  |  | \\ |   /_\\   |       /",
			"| \\   /     |	  |__|  |   |  /   \\  |__|   /"
		);
		const name_t items[] = { "start", "settings", "exit" };
		print_list(pos.y, 3, items, 0, 3);
	}

	return gupdate_c(CONTINUE, NULL);

}

void start_app(void)
{
	setlocale(LC_ALL, "rus");
	system("mkdir data");

	gparam_t param =
	{
		.field = {.name = (char*)calloc(261, sizeof(char))},
		.current_cell = (tuple_t*)calloc(1, sizeof(tuple_t)),
	};
	strcpy(param.field.name, "data.txt");
	file_data(&param, TRUE);

	for (int i = 0; i < param.field.size; i++)
	{
		for (int k = 0; k < param.field.size; k++)
			*(param.field.array + (i * param.field.size + k)) = cell_c(0, param.field.size, DEFAULT);
	}//------------------------------------------------------------------------------------------------------------------------------------
	update_frame(mainmenu, tuple_c(0, 3), FALSE, &param);

}

bool file_data(field_t* field, bool readonly)
{
	name_t dirname;
	FILE* file;

	if(!readonly)strcat(field->name, ".txt");

	sprintf(dirname, "./data/%s", field->name);
	if (!(file = fopen(dirname, readonly ? "rt" : "w"))) return FALSE;

	if (readonly) 
	{
		int index = 0;
		fscanf(file, "%d", &(field->size));
		field->array = (cell_t*)calloc(pow(field->size, 2),sizeof(cell_t));

		for (int i = 0; i < pow(field->size, 2); i++)
		{
			(field->array + i)->free_value.x = (field->array + i)->free_value.y = field->size;
			(field->array + i)->color = DEFAULT;
		}
		while (!feof(file))fscanf(file, "%d", &((field->array + (index++))->check_value));
	}
	else
	{
		
		fprintf(file, "%d\n", field->size);
		for (int i = 0; i < field->size; i++)
		{
			for (int k = 0; k < field->size; k++)
				fprintf(file, "%d\t", (field->array + (i * field->size + k))->check_value);
			fprintf(file, "\n");
		}
	}

	fclose(file);
	return TRUE;
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

gupdate_t game_loop(int size, void* args[], gaction_t a, tuple_t pos)
{
	static int life_counter = LIFE_COUNT;
	char* life_line = (char*)calloc(life_counter * 3 + 1, sizeof(char));
	for (int i = 0; i < life_counter; i++) strcat(life_line, "\x03  ");

	field_t* field = (field_t*)args;

	printf("\n[ LIFE: %8.8s ]\n", life_line);
	if (a == ACCEPT) 
	{
		bool check = check_axis(pos, args); 
		/*if (!check)
		{
			(field->array + (pos.y * field->size + pos.x))->color = RED;
		}*/
	}
	else if (a == BACK)
	{
		dir_t items = dir_c((name_t*)calloc(2, sizeof(name_t)), 2);
		strcpy(items.array, "continue");
		strcpy(items.array + 1, "leave");

		switch (*(int*)update_frame(dialog_box, tuple_c(0, 3), FALSE, &items))
		{
		case 0: return gupdate_c(RETURN_TO_BEGIN, NULL);
		case 1: return gupdate_c(EXIT, NULL);
		}
	}
	else
	{
		bool trigger = FALSE;
		switch (draw_field(pos, field))
		{
		case WIN: trigger = TRUE;
		case LOSE:
			if (--life_counter <= 0 || trigger == TRUE)
			{
				clear_frame();
				printf("\n\n\n\t\tYOU %s\n", life_counter <= 0 ? "LOSE" : "WIN");
				getch();
				life_counter = LIFE_COUNT;
				return gupdate_c(EXIT, TRUE);
			}
			check_axis(pos, field);

			return gupdate_c(RETURN_TO_BEGIN, NULL);
		case RUNNING:break;
		}
	}
	return gupdate_c(CONTINUE, NULL);
}

void set_line(int dir, int* last, int i, int * cell) // перезапись значений свобоных клеток выбранной оси
{
	int value = i - (*last);
	for (; (*last) < i; (*last)++, cell += (4*dir)) *cell = value;
	(*last)++;
}

bool check_axis(tuple_t pos, field_t* ptr_param) //проверка по двум осям на пересечении x and y
{
	cell_t *cell = (ptr_param->array + (pos.y * ptr_param->size + pos.x));

	if (cell->check_value > 0) return ;
	cell->check_value = (cell->check_value == BLACK_CELL ? WHITE_CELL : BLACK_CELL);

	bool result = TRUE;

	tuple_t last = tuple_c(0,0);
	for (int i = 0; i <= ptr_param->size; i++)
	{
		if ( i != pos.x && (ptr_param->array + (pos.y * ptr_param->size + i))->check_value
			== (ptr_param->array + (pos.y * ptr_param->size + pos.x))->check_value)result = FALSE;

		if ((ptr_param->array + (pos.y * ptr_param->size + i))->check_value == BLACK_CELL
			|| i == ptr_param->size)
		{
			set_line(1, &last.x, i, &((ptr_param->array + 
				(pos.y * ptr_param->size + last.x))->free_value.x));
		}
			
		if ((ptr_param->array + (i * ptr_param->size + pos.x))->check_value == BLACK_CELL
			|| i == ptr_param->size)
		{
			set_line(ptr_param->size, &last.y, i, &((ptr_param->array + 
				(last.y * ptr_param->size + pos.x))->free_value.y));
		}     
	}
	return result;
}

gstate_t draw_field(tuple_t pos, field_t *param )
{
	bool trigger = TRUE;
	gstate_t result = RUNNING;
	cell_t* cell;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	
	printf("\n\n");
	for (int y = 0; y < param->size; y++)
	{
		for (int x = 0; x < param->size; x++)
		{
		cell = (param->array + (y * param->size + x));
		if (pos.x == x && pos.y == y) SetConsoleTextAttribute(console, 112);
	/*	else SetConsoleTextAttribute(console, cell->color);*/
		if (cell->check_value > 0)
		{
			if (cell->free_value.x + cell->free_value.y < cell->check_value + 1)
			{
	/*			SetConsoleTextAttribute(console, 64);*/
				result = LOSE;
			}
			else if (cell->free_value.x + cell->free_value.y != cell->check_value + 1)
				trigger = FALSE;
			//else if (cell->free_value.x + cell->free_value.y == cell->check_value + 1)
			//	SetConsoleTextAttribute(console, 32);
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
