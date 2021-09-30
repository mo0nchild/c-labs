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