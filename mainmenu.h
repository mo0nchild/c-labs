#pragma once

#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <malloc.h>

#include <stdlib.h>
#include <Windows.h>
#include <string.h>


#include <dirent.h>
#include "coursework.h"

GAME_STATE test(POSITION pos, SETTING_PARAM* param)
{
	_Bool trigger = TRUE;
	GAME_STATE result = RUNNING;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	printf("\n\n");

	for (int y = 0; y < param->field_size; y++)
	{
		for (int x = 0; x < param->field_size; x++)
		{
			if (pos.x == x && pos.y == y) SetConsoleTextAttribute(console, 112);
			if ((param->field + (y * param->field_size + x))->check_value > 0)
			{
				printf("|%5d|", (param->field + (y * param->field_size + x))->check_value);
			}
			else printf("|%5c|", (param->field + (y * param->field_size + x))
				->check_value == WHITE_CELL ? ' ' : 'X');

			SetConsoleTextAttribute(console, 7);
		}
		printf("\n\n");
	}

	if (trigger) result = WIN;
	return result;
}

SCENES mainmenu(void)
{
	ACTIONS input = TRUE_T;
	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
	POSITION pos = set_position(0, 0);
	char items[][15] = { "start\0", "settings\0", "exit\0" };
	while (1) 
	{
		switch (input) 
		{
		case ACCEPT: return (SCENES)pos.y;
		case TRUE_T:
			CLEAR_FRAME();
			
			printf("\n\n");
			for (int i = 0; i < 3; i++) 
			{
				if(pos.y == i)SetConsoleTextAttribute(c, 112);
				printf("\n\t\t%10s\n", items[i]);
				SetConsoleTextAttribute(c, 7);
			}
			break;
		case FALSE_T: break;
		}
		input = get_keyboard_input(&pos, set_position(0, 3));
	}
}

void set_cell_value(POSITION cell, SETTING_PARAM * param) 
{
	ACTIONS input = TRUE_T;
	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
	POSITION pos = set_position(0, 0);
	int value, max = param->field_size*2-1;

	bool checker = FALSE;
	while (1)
	{
		switch (input)
		{
		case ACCEPT:
			(param->field + (cell.y * param->field_size + cell.x))->check_value = value;

			checker = TRUE;
		case TRUE_T:
			CLEAR_FRAME();
			value = pos.x+1;
			printf("\n\n\n\n\tset cell: %c %2d %c",
				(value == 1) ? ' ' : '<',
				value, (value == max) ? ' ' : '>'
			);
		case FALSE_T: break;
		}
		if (checker) break;
		input = get_keyboard_input(&pos, set_position(max, 0));
	}
}

void save_file(SETTING_PARAM * param) 
{
	CLEAR_FRAME();
	printf("\n\n\tfile name: ");
	scanf("%s", param->filename);
	strcat(param->filename, ".txt");
	file_data(param, FALSE);
}

void edit_field(SETTING_PARAM * param) 
{
	ACTIONS input = TRUE_T;
	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
	POSITION pos = set_position(0, 0);
	int value = 0;

	pos = set_position(0, 0);
	input = TRUE_T;
	bool checker = FALSE;
	while (1)
	{
		switch (input)
		{
		case ACCEPT:
			param->field_size = value;
			param->field = realloc(param->field, sizeof(CELL) * pow(value, 2));
			for (int i = 0; i < param->field_size; i++)
			{
				for (int k = 0; k < param->field_size; k++)
					*(param->field + (i * param->field_size + k)) = set_cell(0, param->field_size);
			}
			checker = TRUE;
		case TRUE_T:
			CLEAR_FRAME();
			value = 4 + pos.x;
			printf("\n\n\n\n\tset field size: %c %2d %c",
				(value == 4) ? ' ' : '<',
				value, (value == 10) ? ' ' : '>'
			);
		case FALSE_T: break;
		}
		if (checker) break;
		input = get_keyboard_input(&pos, set_position(7, 0));
	}
	pos = set_position(0, 0);

	input = TRUE_T;
	checker = FALSE;
	while (1)
	{
		switch (input)
		{
		case BACK:
			save_file(param);
			checker = TRUE;
			break;
		case ACCEPT:
			set_cell_value(pos, param);
		case TRUE_T:
			CLEAR_FRAME();
			test(pos, param);
		case FALSE_T: break;
		}
		if (checker) break;
		input = get_keyboard_input(&pos, set_position(param->field_size, param->field_size));
	}
}

void print_rules(SETTING_PARAM * param) 
{
	CLEAR_FRAME();
	printf("%s\n\n", param->filename);
	printf("\t уромасу играетс€ на пр€моугольной сетке. \n¬ некоторых из этих €чеек есть числа. \n ажда€ €чейка может быть черной или белой. \n÷ель состоит в том, чтобы определить, \nк какому типу относитс€ кажда€ €чейка.\n\t—ледующие правила определ€ют, какие €чейки какие :\n\t1) аждое число на доске представл€ет количество белых клеток, \nкоторые можно увидеть из этой клетки,\n включа€ ее самого.ячейку можно увидеть из другой €чейки,\n если они наход€тс€ в той же строке или столбце,\n и между ними нет черных €чеек в этой строке или столбце.\n\t2)ѕронумерованные €чейки не могут быть черными.\n\t3)Ќикакие две черные клетки не могут быть смежными по горизонтали или вертикали.\n\t4)¬се белые клетки должны быть соединены горизонтально или вертикально.\n");
	getch();
}

void load_file(SETTING_PARAM * param) 
{
	DIR* d = opendir(".\\data");
	struct dirent* dir;

	char files[20][261];
	int index = 0;

	if (d) {
		while ((dir = readdir(d)) != NULL && index < 20) {
			strcpy(files[index++], dir->d_name);
		}
		closedir(d);
	}
	
	ACTIONS input = TRUE_T;
	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
	POSITION pos = set_position(0, 0);
	bool checker = FALSE;

	while (checker != TRUE_T) 
	{
		switch (input)
		{
		case BACK:

			checker = TRUE;
			continue;
		case ACCEPT:
			param->filename = (char*)realloc(param->filename, sizeof(char) * 261);
			strcpy(param->filename, files[pos.y + 2]);
			checker = TRUE;
			continue;
		case TRUE_T:
			CLEAR_FRAME();
			printf("\n\n");
			for (int i = 2; i < index; i++)
			{
				if (pos.y == i-2)SetConsoleTextAttribute(c, 112);
				printf("\n\t%15s\n", files[i]);
				SetConsoleTextAttribute(c, 7);
			}
			printf("\n");
		case FALSE_T: break;
		}
		input = get_keyboard_input(&pos, set_position(0, index-2));
		
	}
	printf("%s", param->filename);
	getch();
}

void settings(SETTING_PARAM* param) 
{
	ACTIONS input = TRUE_T;
	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
	POSITION pos = set_position(0, 0);
	int value = 0;

	pos = set_position(0, 0);
	input = TRUE_T;
	bool checker = FALSE;
	const char items[][15] = {"edit\0", "load\0", "rules\0"};
	while (1)
	{
		switch (input)
		{
		case BACK:
			
			checker = TRUE;
			break;
		case ACCEPT:
			switch (pos.y) 
			{
			case 0: edit_field(param); break;
			case 1: load_file(param); break;
			case 2: print_rules(param);  break;
			}

		case TRUE_T:
			CLEAR_FRAME();
			printf("\n\n");
			for (int i = 0; i < 3; i++) 
			{
				if (pos.y == i)SetConsoleTextAttribute(c, 112);
				printf("\n\t\t%10s\n", items[i]);
				SetConsoleTextAttribute(c, 7);
			}
			printf("\n");
		case FALSE_T: break;
		}
		if (checker) break;
		input = get_keyboard_input(&pos, set_position(0, 3));
	}
}

void RUN_(void) 
{
	SETTING_PARAM param = {
		.field_size = 7,
		.filename = (char*)calloc(261, sizeof(char)),
		.field = (CELL*)calloc(49, sizeof(CELL)),
	};

	strcpy(param.filename, "data.txt");
	file_data(&param, TRUE);

	for (int i = 0; i < param.field_size; i++)
	{
		for (int k = 0; k < param.field_size; k++)
			*(param.field + (i * param.field_size + k)) = set_cell(0, param.field_size);
	}
	while (1) 
	{
		switch (mainmenu()) 
		{
		case 0: run_app(&param); break;
		case 1: settings(&param); break;
		case 2: return;
		}
	}

}

void start_app(void) 
{
	system("mkdir data");
	RUN_();
}