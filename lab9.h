#pragma once

#include <stdio.h>
#include <math.h>

#define ROW 9
#define COL 9

enum KEYS { ENTER = 13, LEFT = 97, RIGHT = 100 };

void task1(void) 
{
    printf("\n");
    for (int row = 1; row <= ROW; ++row)
    {
        for (int col = 1; col <= row; ++col) // COL -> row
        {
            printf("%5d", col*row);
        }
        printf("\n");
    }
}

int set_value(const char str[], int * id)
{
    printf("%s", str);
    return scanf("%d", id);
}

void task2(void) 
{
    int begin, end, counter = 1;
    if (!set_value("введите начально число: ", &begin) || 
        !set_value("введите конечное число: ", &end))
    {
        return printf("\nнеправильный ввод\n\n");
    }

    for (int row = begin; row <= end; ++row)
    {
        for (int col = 1; col <= counter; ++col)
        {
            printf("%5d", row * 10);
        }
        counter++;
        printf("\n");
    }
    printf("\n\n");
}

void draw_figure(double A) 
{

    double R = sqrt(5.) / (sqrt(5.) + 1) * A,
        B = sqrt(R * R - pow(A - R, 2)),
        H1 = A * (sqrt(5) - 1) / (sqrt(5) + 1),
        H2 = A * (sqrt(5)) / (sqrt(5) + 1);

    printf("R=%.1lf A=%.1lf\n", R, A);
    int delta = 0;

    for (int y = 0; y < 2 * R; y++)
    {
        delta = sqrt(pow(R, 2) - pow(R - y, 2));

        for (int x = 0; x < 2 * R; x++)
        {
            // отрисовка круга 
            if (x == (int)(R - delta) || x == (int)(delta + R))printf("\' ");
            else if (x < delta + R) printf("  ");

            // отрисовка прямоугольника
            if (y <= (int)A && x >= (int)(R - B + 1) && x <= (int)R)
            {
                if (y == 0 || y == (int)A) printf("\b\b* ");
                else if ((y == (int)H1 || y == (int)H2) &&
                    (x > (int)(R - B + 1) && x < (int)R))printf("\b\b\' ");
                else
                {
                    if (x == (int)(R - B + 1) || x == (int)R) printf("\b\b* ");
                    if (y < (int)H2 && y >(int)H1)
                    {
                        int j = (int)((y - H1) / (H2 - H1) * B + R - B);
                        if (x <= j + 1 && x >= j) printf("\b\b\' ");
                    }
                }
            }
        }

        printf("\n");
    }
}

void hometask(void) 
{
    int key;
    double A = 15;
    while (1)
    {
        system("cls");
        printf("\rвведите значение стороны:\t\t<< %.1lf >>\n\tkey_a=уменьшить key_d=увеличить\n", A);
        draw_figure(A);
        
        key = getch();

        switch (key)
        {
        case ENTER:
            return;
        case LEFT:
            A = (A < 15 ? A : A - 5);
            break;
        case RIGHT:
            A = (A > 40 ? A : A + 5);
        default: break;
        }
    }
    
}