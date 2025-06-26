//5번문제

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	char character;
	printf("문자를 입력하시오: ");
	scanf("%c", &character);
	printf("3개의 후속 문자들: %c, %c, %c\n", character + 2, character + 4, character + 6);

	return 0;
}