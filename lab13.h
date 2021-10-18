#pragma once
#include <stdio.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>

void task1(void) 
{
    char name[20] = "", hello[40] = "hello user,\0";
    printf("input username: ");
    gets(name);

    name[0] = toupper(name[0]);
    for (int i = 1; i < strlen(name); i++) 
    {
        name[i] = tolower(name[i]);
    }

    if (!strcat(hello, name)) return printf("\n\tERROR\n");
    printf("%s\n", hello);

    for (int i = 0; i < strlen(hello); i++) 
    {
        hello[i] = toupper(hello[i]);
    } printf("%s\n", hello);

    /*printf("%d", strlen(hello));*/

    const int sizse = 40 - strlen(hello);
    char *str = (char*)malloc(sizeof(char) * sizse);
    for (int i = 0; i < sizse-1; i++) str[i] = '!';
    str[sizse-1] = '\0';
    strcat(hello, str);
    printf("%s\n", hello);
}

void task2(void) 
{
    char name[20] = "", hello[40] = "hello user,\0";
    printf("input username: ");
    gets(name);

    char name1[20];
    
    name[0] = toupper(name[0]);
    for (int i = 1; i < strlen(name); i++)
    {
        name[i] = tolower(name[i]);
    }
    strcat(hello, name);

    printf("\t%s\n", hello);

    strcpy(name1, name); strcpy(hello, "hello user,\0");
    for (int i = 0; i < strlen(name); i++) name[strlen(name) - 1 - i] = name1[i];
    strcat(hello, name);

    printf("\t%s\n", hello);

    
}

void task3() 
{
    char str[50];
    printf("input value: ");
    gets(str);

    int counter = 0;

    for (int i = 0; i < strlen(str) - 1; i++) 
    {
        if (str[i] == str[i + 1]) counter++;
    }


    printf("Одинаковых соседей: %d", counter);
}