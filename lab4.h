#pragma once
#include <stdio.h>
#include <locale.h>

void task1(void) 
{
	double input_double = 10.123123;
	int input_int = 100;
	float input_float = 10.67f;
	char input_char = 'a';

	printf("int: %d\nfloat: %f\ndouble: %lf\nchar: %c\n\n",
		input_int, input_float, input_double, input_char);

	printf("введите int: ");
	scanf("%d", &input_int);
	printf("введите double: ");
	scanf("%lf", &input_double);
	printf("введите float: ");
	scanf("%f", &input_float);
	printf("введите char: ");
	scanf(" %c", &input_char);

	printf("int: %d\nfloat: %f\ndouble: %lf\nchar: %c",
		input_int, input_float, input_double, input_char);

}

void task2(void) 
{
	int a = 11, b = 3;
	/*int x;
	float y;
	double z;

	x = a / b;
	y = a / b;
	z = a / b;

	printf("x: %d\t y: %f\tz: %lf\n\n", x, y, z);
	printf("(double)a/b: %lf\n(double)(a/b): %lf\na/(double)b: %lf\n\n",
		(double)a/b,
		(double)(a/b),
		a/(double)b
	);

	printf("(float)a / b: %f\n(float)(a / b): %f\na / (float)b: %f\n",
		(float)a / b,
		(float)(a / b),
		a / (float)b
	);*/

	printf("int: %d\nfloat: %f\ndouble: %lf\n\n",
		a/b,
		a/b*1.f,
		a/b
	);

}

void task3(void) 
{
	int n;
	printf("введите число: ");
	if(!scanf("%d", &n))
		return printf("\nвведено не число\n\n");

	if (n < 100 || n > 999) 
		return printf("\nчисло не подходит\n");

	int n1 = n / 100, n2 = (n / 10) % 10, n3 = n % 10;

	printf("\nпоследняя цифра: %d\nпервая цифра: %d\nсумма: %d\n\n", 
		n3, n1, n1 + n2 + n3);
	printf("наоборот: %d\n",n1+ n2 * 10 + n3*100);
}

void task4(void)
{
	double v1, t1, v2, t2, v3, t3;

	printf("(предупреждение! ввод осуществляется в формате: V t\nвведите значения для первого сосуда: ");
	if (!scanf("%lf%lf", &v1, &t1)) return printf("\nнеправильный ввод\n\n");

	printf("введите значения для второго сосуда: ");
	if (!scanf("%lf%lf", &v2, &t2)) return printf("\nнеправильный ввод\n\n");
	
	printf("введите значения для третьего сосуда: ");
	if (!scanf("%lf%lf", &v3, &t3)) return printf("\nнеправильный ввод\n\n");

	printf("\nобъем: %.2lf\tтемпература: %+.2lf\n\n", 
		v1+v2+v3,
		(t1+t2+t3)/3
	);
}