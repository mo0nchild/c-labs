#pragma once
#include <stdio.h>
#include <math.h>

#define M_PI            3.14159265358979323846

void task1(void) 
{
	double input;
	printf("������� ���� (� ��������): ");

	scanf("%lf", &input);
		/*return printf("\n� ������ ����� �������� ��������\n\n");*/
	
	printf("\nsin: %.6lf\n\n", sin(input*M_PI/180));
}

void task2(void)
{
	double k = 7., x, y;
	printf("������� x: ");
	scanf("%lf", &x);

	double a = pow(k + x, 3),
		b = log10(fabs(x * k));

	printf("\ny: %.3lg\n\n", pow(b, 2) + sqrt(fabs(a)));

}

void task3(void)
{
	double k = 7., x = 2.2;
	int A = pow(k + x, 3), 
		B = log10(fabs(x * k)),
		C = (pow(B, 2) + sqrt(fabs(A)));
	char str1[50], str[50];

	(A % 2 != B % 2) ?
		sprintf(str1, "������ ���� �� ����� %d � %d ������\n\n", A, B)
		: ((A % 2 == 0) ?
			sprintf(str1, "��� ����� %d � %d ������\n\n", A, B):
			sprintf(str1, "��� ����� %d � %d ��������\n\n", A, B)
			);


	printf("\nA: %d\tB: %d\tC: %d\na) %sb) %s\n\n", A,B,C,
		str1, (A % 3 == 0 && B % 3 == 0 && C % 3 == 0) ?
		"������ �� ����� ������ ����" :
		"����� �� ����� �� ������ ����"
		
	);

}

double F(double x, double y) 
{
	double t = 0.5;
	return (
		(pow(sin(x), 3) + log(2 * y + 3 * x)) / 
		(pow(t, exp(1)) + sqrt(x))
	);
}

void task4(void) 
{
	printf("\n�����1: %.6lf\n�����2: %.6lf\n\n",
		F(2, 1.5 * pow(10,-6)),
		F(4,2)
	);
}