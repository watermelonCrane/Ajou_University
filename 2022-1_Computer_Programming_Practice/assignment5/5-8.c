//8번문제
#include<stdio.h>

int main() {
	int a;
	printf("정수를 입력하시오: ");
	scanf("%d", &a);
	do {
		printf("%d", a % 10);
		a /= 10;
	} while (a);
	printf("\n");

	return 0;
}