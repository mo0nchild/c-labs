#pragma once

#include <stdio.h>
#include <math.h>

int input_action(const char msg[], int * n) 
{
	printf("%s", msg);
	return scanf("%d", n);
}

double func(double x)
{
	return fabs(pow(exp(1), x) - 2) - x * x;
}

enum KEYS { ENTER=13, LEFT=97,RIGHT=100 };

double select_value(const char msg[], double step) 
{
	int key = 0;
	double result = 1;
	// right=100 left=97 enter=13

	printf("\n");
	while (1) 
	{
		printf("\r\t%s\t\t<< %.1lf >> ", msg, result);
		key = getch();

		switch (key) 
		{
		case ENTER: 
			printf("\n");
			return result;
		case LEFT:
			result = (result < 0.2 ? result : result-step);
			break;
		case RIGHT:
			result = (result > 9 ? result : result+step);
		default: break;
		}
	}
}

void task1(void) 
{
	int n, m = 0, s = 0;

	if (!input_action("������� ��������� �����: ", &n) || 
		!input_action("������� �������� �����: ", &m))
	{
		return printf("\n������� �� �����\n\n");
	}

	for (int i = m; i >= n; --i)
	{
		s += i;
		printf("\n��������� %d ���", abs(i - m - 1));
	}
	printf("\n\n�����: %d\n\n", s);

}

void task2(void)
{
	printf("f(x) = | e^x - 2 | - x^2\n\n");
	
	double tab_value = select_value("������� �������� ��������", 1),
		step = select_value("������� �������� ����", .1);

	double a, b;
	char wall1, wall2;
	printf("\n\n������� �������� ���������.\n������: (2,1;2,6) or [1,2;2,6]\n");

	while (1) 
	{
		if (scanf("%c%lf;%lf%c", &wall1, &a, &b, &wall2))
		{
			if ((wall1 == '[' && wall2 == ']') ||
				(wall1 == '(' && wall2 == ')')) break;
		}
		printf("->\t%c%lf;%lf%c\n������� �������\n\n", wall1, a, b, wall2);
	}

	char tabs[15], line1[50], line2[50];
	int index;

	for (index = 0; index < tab_value; index++) tabs[index] = ' ';
	tabs[index] = '\0';

	for (double i = (wall1 == '(' ? a + step : a); (wall2 == ')' ? i < b : i <= b); i += step) 
	{
		sprintf(line1, "|%s%.1lf%s\0", tabs, i, tabs);
		sprintf(line2, "|%s%.1lf%s\0", tabs, func(i), tabs);
		printf("%s%s|\n", line1, line2);
	}
	
}

//	��������� � �������������� ����� for �������� ����, ��� ��������
//	������������� �������� a � n
//										a(a + 1) ... (a + n + 1)

void task3(void)
{
	double a, n, s;
	printf("������� �: ");
	if (!scanf("%lf", &a)) return printf("\n�����������\n\n");
	printf("������� n: ");
	if (!scanf("%lf", &n)) return printf("\n�����������\n\n");

	s = a;
	for (int i = 0; i <= n; i++) s *= (a + 1 + i);
	printf("\n\t���������: %.1lf\n\n", s);
}