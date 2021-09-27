#pragma once
#include <stdio.h>

void task1(void) 
{
	char c;
	printf("введите символ: ");
	scanf("%c", &c);
	printf("\nвведено: %c\t",c);

	switch (c) 
	{
	case '0':case '1':case '2':
	case '3':case '4':case '5':
	case '6':case '7':case '8':
	case '9': printf("это цифра\n");break;
	default: printf("это буква\n");break;
	}
}

void task2(void)
{
	float a, b, result = 0.f;
	char _operator;
	printf("введите выражение: (например: 1+1)\n");
	scanf("%f%c%f", &a,&_operator,&b);
	switch (_operator) 
	{
	case '+': result = a + b; break;
	case '-': result = a - b; break;
	case '*': result = a * b; break;
	case '/': result = a / b; break;
	case '^': result = pow(a,b); break;
	default: printf("неверный оператор\n"); return;
	}

	printf("=%.2f\n", result);
}

void task3_english(void)
{
	char input;
	printf("введите букву английского алфавита: ");
	scanf("%c", &input);

	if ((int)input - 96 < 1 || (int)input - 96 >26)
	{
		printf("неизвестный символ\n");
		return;
	}

	printf("%d\n", (int)input-96);
}

struct bykva 
{
	int place;
	char value;
};

const int alphavit[] = {
	-96, -95, -94, -93, -92, -91, -15, -90, 
	-89, -88, -87, -86, -85, -84, -83, -82, 
	-81, -32, -31, -30, -29, -28, -27, -26, 
	-25, -24, -23, -22, -21, -20, -19, -18, -17, 
};

void task3_rus(void) 
{
	char input;
	printf("введите букву русского алфавита: ");

	if (!scanf("%c", &input)) 
	{
		printf("\nвведена не буква\n\n");
		return;
	}
	for (int i = 0; i < 33; ++i) 
	{
		if ((int)input == alphavit[i]) 
		{
			printf("\nномер: %d\n\n", ++i);
			return;
		}
	}

	printf("\nнет такой буквы в алфавите\n\n");
}
