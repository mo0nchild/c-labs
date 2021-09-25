#pragma once
#include <stdio.h>
#include <math.h>

void task1(void) 
{
	int year;
	printf("������� ���: ");
	scanf("%d", &year);

	if (year % 4 == 0) 
	{
		printf("\n1) ��� %d ����������\n\n", year);
	}
	else printf("\n1) ��� %d �� ����������\n\n", year);

	if ((year % 4 == 0 && year % 100 != 0) ||
		year % 400 == 0) printf("2)��� %d ����������\n\n", year);
	else printf("2)��� %d �� ����������\n\n", year);
}

double F(x) 
{
	return (
		(x <= 2) ? pow(x,2) + 4 * x + 5:
			1./ (pow(x, 2) + 4 * x + 5)
	);
}

void task2(void)
{
	double x;
	printf("������� x: ");
	scanf("%lf", &x);

	printf("\nF = %.2lf\n\n", F(x));
}

void task3(void)
{
	double x1, x2, x3;
	printf("������� �������� ��� ������ ����������: ");
	scanf("%lf", &x1);

	printf("������� �������� ��� ������ ����������: ");
	scanf("%lf", &x2);

	printf("������� �������� ��� ������� ����������: ");
	scanf("%lf", &x3);

	double output;

	if (x1 > x2)
	{
		if (x1 > x3 && x3 <= x2)output = x2 * x1;
		else output = x3 * x1;
	}
	else
	{
		if (x2 > x3 && x3 <= x1)output = x2 * x1;
		else output = x3 * x2;
	}
	printf("\n�����: %.2lf\n", output);

	//-------------------------------------------------

	double outs[3] = { x1, x2, x3 }, buffer;
	for (int i = 0; i < 3; i++)
	{
		for (int k = i + 1; k < 3; k++) 
		{
			if (outs[i] < outs[k]) 
			{
				buffer = outs[i];
				outs[i] = outs[k];
				outs[k] = buffer;
			}
		}
	}

	printf("\n�����: %.2lf\n", outs[0] * outs[1]);

}