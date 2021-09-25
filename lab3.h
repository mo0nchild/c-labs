#pragma once
#include <stdio.h>

#define D 2.54
#define P 2.32166

void task1(void)
{
    int input;
    puts("������� �����:");
    scanf("%d", &input);
    printf("������� ����� %d", input);
}

void task2(void)
{
    int dym;

    puts("������� ��������");
    scanf("%d", &dym);
    float result = D * dym;

    printf("%d ������ - ��� %.1f ��\n%d ���������� - ��� %.1f ��\n%d ��������������� - ��� %.1f ��",
        dym, result,
        dym, P * dym,
        dym, dym * 2.7076);

    //������������

   /* puts("������� �������� ����\n1- ������� ����\n2 - ���������� ����\n3 - ������� ����\n4 - ������������ ����\n5 - �������������� ����\n��������: (10 5)\n");
    int mode;
    scanf("%d %d", &dym, &mode);
    switch (mode)
    {
    case 1: printf("������� ���� = %.1f��", dym * 1.852f); break;
    case 2: printf("���������� ���� = %.1f��", dym * 1.609f); break;
    case 3: printf("������� ���� = %.1f��", dym * 1.475f); break;
    case 4: printf("������������ ���� = %.1f��", dym * 7.468f); break;
    case 5: printf("�������������� ���� = %.1f��", dym * 7.412f); break;
    default:puts("����������� �������� ������");break;
    }*/
}

void task3(void)
{
    double a, b;
    puts("������� �������� a � b");
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

#include <locale.h> // ����������� ������������ ������

int get_result(int n) // ������� ��� ������� ����� �������� ����� n
{
    return ((n % 10) + ((n / 10) % 10) + (n / 100));
}

void dz(void) // ������� �������
{
    setlocale(LC_ALL, "RUS"); // ����������� (��������� ������� �������� � �������)
    int n; // ���������� ���������� n 

    puts("������� �����:"); // ������ �� ������� �������� �� ������������ 
    int check = scanf("%d", &n); // ���������� �������� � ���������� n 

    if (!check) { // ��������, ��� ������������ ���� ����� �����
        printf("\n������� �� �����\n");
        return;
    }

    if (100 <= n && n <= 999) // �������� �� �������������
    {
        printf("\n���������:\t%d\n", get_result(n));
        // ����� ����������, ������������ � ������� get_result 
        // (���������� ����� �������� ����� n - �����������)
    }
    else puts("\n������� �� ����������� �����\n");
    // ����� ��������� ������
}