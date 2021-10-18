#pragma once

#include <stdio.h>
#include <math.h>

double fa(double x) 
{
	double y = (x<=3) ? x*x-3*x+9 : 1./(pow(x,3) + 3);
	return y;
}

double fb(double x) 
{
	return x * pow(exp(1), sin(x * x));
}
double sin_n(double x, int N);

void task1(void) 
{
	double x;
	printf("введите значение x: ");
	scanf("%lf", &x);

	printf("\nfa(x)=%lf , fb(x)=%lf\n\n", fa(x), fb(x));
	printf("произведение: %lf\nразность квадратов %lf\nудвоенная сумма %lf\n\n", 
		fa(x) * fb(x), pow(fa(x), 2) - pow(fb(x), 2), 2 * (fa(x) + fb(x))
	);

}




double factorial(double x) 
{
	if (x == 0) return 1;
	return (factorial(x - 1) * x);
}

double cos_n(double x, int N) 
{
	double result = 1;
	for (int n = 1; n < N; n++) 
	{
		result += (pow(-1, n) * pow(x, 2.*n)/ (factorial(2 * n)));
	}
}

double cos_eps(double x, double eps) 
{
	double result = 1, last_value = 1;
	int n = 1;
	while (fabs(result - last_value) <= eps) 
	{
		last_value = result;
		result += (pow(-1, n) * pow(x, 2. * n) / (factorial(2 * n)));
		n++;
	}
	return result;
}

#define EPS .02
void task2(void) 
{
	printf("input: ");
	double x; int N;
	scanf("%lf %d", &x, &N);
	for (int N = 1; N < 10; N++) 
	{
		printf("|%10d|%10.4lf|%10.4lf|%10.4le|%10.4lf|\n", N, cos(x), cos_n(x, N), fabs(cos(x) - cos_n(x, N)), cos_eps(x, EPS));
	}
}

float area_rectangle(float a, float b) 
{
	return (a * b);
}

void name(void) 
{
	printf("прямоугольник\n");
}

void draw_rectangle(int a, int b) 
{
	for (int i = 0; i < a; i++) 
	{
		for (int k = 0; k < b; k++) printf(" * ");
		printf("\n");
	}
	printf("\n\n\n");
}

void task3(void) 
{
	printf("input: ");
	int x;
	double a, b;
	scanf("%d", &x);

	printf("input a & b: ");
	scanf("%lf %lf", &a, &b);

	switch (x) 
	{
	case 1:printf("%lf\n", area_rectangle(a, b)); break;
	case 2:name(a, b); break;
	case 3:draw_rectangle(a,b); break;
	default: printf("not found\n");
	}
}