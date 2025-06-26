//3번문제

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	int x, y, temp;
	printf("2개의 정수(x, y)를 입력하시오: ");
	scanf("%d %d", &x, &y);
	printf("입력된 정수: x = %d, y = %d\n", x, y);

	temp = x;
	x = y;
	y = temp;

	printf("교환된 결과: x = %d, y = %d\n", x, y);

	return 0;
}