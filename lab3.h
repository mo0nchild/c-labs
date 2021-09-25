#pragma once
#include <stdio.h>

#define D 2.54
#define P 2.32166

void task1(void)
{
    int input;
    puts("Введите число:");
    scanf("%d", &input);
    printf("Введено число %d", input);
}

void task2(void)
{
    int dym;

    puts("Введите значение");
    scanf("%d", &dym);
    float result = D * dym;

    printf("%d дюймов - это %.1f см\n%d испанского - это %.1f см\n%d старолитовского - это %.1f см",
        dym, result,
        dym, P * dym,
        dym, dym * 2.7076);

    //альтернатива

   /* puts("Введите значение мили\n1- морская миля\n2 - сухопутная миля\n3 - римская миля\n4 - старорусская миля\n5 - географическая миля\nНапример: (10 5)\n");
    int mode;
    scanf("%d %d", &dym, &mode);
    switch (mode)
    {
    case 1: printf("морская миля = %.1fкм", dym * 1.852f); break;
    case 2: printf("сухопутная миля = %.1fкм", dym * 1.609f); break;
    case 3: printf("римская миля = %.1fкм", dym * 1.475f); break;
    case 4: printf("старорусская миля = %.1fкм", dym * 7.468f); break;
    case 5: printf("географическая миля = %.1fкм", dym * 7.412f); break;
    default:puts("Неизвестное значение режима");break;
    }*/
}

void task3(void)
{
    double a, b;
    puts("Введите значение a и b");
    scanf("%lf %lf", &a, &b);

    char line[] =
        "------------------------------------------------------------------------------------------------";

    printf("%s\n|%s|%s|%s|\n%s\n",
        line,
        "\t\ta*b\t\t", "\t\ta+b\t\t", "\t\ta-b\t\t",
        line);
    char s1[50], s2[50], s3[50];
    sprintf(s1, "%7.3lf*%7.3lf", a, b);
    sprintf(s2, "%7.3lf+%7.3lf", a, b);
    sprintf(s3, "%7.3lf-%7.3lf", a, b);

    printf("|\t%.15s\t\t|\t%.15s\t\t|\t%.15s\t\t|\n%s", s1, s2, s3, line);

    sprintf(s1, "%7.3lf", a * b);
    sprintf(s2, "%7.3lf", a + b);
    sprintf(s3, "%7.3lf", a - b);
    printf("\n|\t\t%.7s\t\t|\t\t%.7s\t\t|\t\t%.7s\t\t|\n%s\n", s1, s2, s3, line);

}

#include <locale.h> // подключение заголовочных файлов

int get_result(int n) // функция для расчета суммы разрядов числа n
{
    return ((n % 10) + ((n / 10) % 10) + (n / 100));
}

void dz(void) // главная функция
{
    setlocale(LC_ALL, "RUS"); // локализация (установка русских символов в консоли)
    int n; // объявление переменной n 

    puts("Введите число:"); // запрос на входное значение от пользователя 
    int check = scanf("%d", &n); // считывание значения в переменную n 

    if (!check) { // проверка, что пользователь ввел целое число
        printf("\nВведено не число\n");
        return;
    }

    if (100 <= n && n <= 999) // проверка на трехзначность
    {
        printf("\nРезультат:\t%d\n", get_result(n));
        // вывод результата, посчитанного в функции get_result 
        // (возвращает сумму разрядов числа n - трехзначное)
    }
    else puts("\nВведено не трехзначное число\n");
    // вывод сообщения ошибки
}