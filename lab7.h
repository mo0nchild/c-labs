#pragma once
#include <stdio.h>

void task1(void) 
{
	char c;
	printf("������� ������: ");
	scanf("%c", &c);
	printf("\n�������: %c\t",c);

	switch (c) 
	{
	case '0':case '1':case '2':
	case '3':case '4':case '5':
	case '6':case '7':case '8':
	case '9': printf("��� �����\n");break;
	default: printf("��� �����\n");break;
	}
}

void task2(void)
{
	float a, b, result = 0.f;
	char _operator;
	printf("������� ���������: (��������: 1+1)\n");
	scanf("%f%c%f", &a,&_operator,&b);
	switch (_operator) 
	{
	case '+': result = a + b; break;
	case '-': result = a - b; break;
	case '*': result = a * b; break;
	case '/': result = a / b; break;
	case '^': result = pow(a,b); break;
	default: printf("�������� ��������\n"); return;
	}

	printf("=%.2f\n", result);
}

void task3(void)
{
	char input;
	printf("������� ����� ����������� ��������: ");
	scanf("%c", &input);

	if ((int)input - 96 < 1 || (int)input - 96 >26)
	{
		printf("����������� ������\n");
		return;
	}

	printf("%d\n", (int)input-96);
}
