#pragma once
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>


//�������� ���������, ������� ������� ����� ���� number.txt � ������� ����� � ���������� � ���� ������������ ����� � ��������� ����.
void task1(void) 
{
	FILE* file = fopen("number.txt", "wb");
	if (!file) return printf("cant open file\n");

	char c;
	printf("file was opened\ninput: ");
	if (!scanf("%c", &c)) return printf("\ninput error\n");
	
	fprintf(file, "%c\n",c);
	fclose(file);

	file = fopen("number.txt", "ab");

	fprintf(file, "%.2lf\n", 12.56);
	fclose(file);
}

//������� 2. ������������ ���������, ������� ���������� � ���� temp.txt, 
//�������� ������������� ���������� ��������� ������������ ����� � ��������� �� -100 �� 100 � ���� �������:
void task2(void) 
{
	FILE* file = fopen("temp.txt", "w");
	if (!file) return printf("cant open file\n");

	srand(time(NULL)); 

	int n;
	printf("input: ");
	scanf("%d", &n);

	char* line = (char *)malloc(50 * n * sizeof(char));
	*line = '\0';

	for (int i = 0; i < n; i++) 
	{
		char str[50];
		sprintf(str, "|%5d|%5d|\n\0", i + 1, rand() % 200 - 100);
		strcat(line, str);
		
	}
	printf("%s\n", line);
	fprintf(file, "%s", line);
	fclose(file);
}


//������� 3. ������������ ���������, ������� ���������� � ���� ���� ��������� ������� 3 ������ 18
typedef struct 
{
	char filmname[20];
	char genre[20];
	int day;
	int mounth;
	int year;
	int hour;
} kinoseans;

int writefile(char* filename, kinoseans* kino, int size) 
{
	FILE* file = fopen(filename, "w");
	if (!file) return 0;


	for (int i = 0; i < size; i++) 
	{
		fprintf(
			file,
			"name:%20s ; genre: %20s ; data : %2d:%2d:%4 ; hour : %d \n\n",
			kino[i].filmname, kino[i].genre, kino[i].day, kino[i].mounth, kino[i].year, kino[i].hour
		);
	}
	fclose(file);
	return 1;
}

void task3(void) 
{
	kinoseans kinos[4] = 
	{ 
		{"SREK", "COMEDY", 23,12,2004, 2},
		{"MIKEYMOUSE", "COMEDY", 3,2,1990, 3},
		{"TOM&JERRY", "ANIME", 2,6,2010, 2},
		{"SREK", "COMEDY", 23,12,2004, 2},
	};

	if (!writefile("file.txt", kinos, 4)) return printf("error\n");
	printf("cool\n");
}
