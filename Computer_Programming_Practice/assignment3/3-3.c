// 3번 문제
#include <stdio.h>

int main(void) {
	int value, value2;
	printf("정수를 입력하시오: ");
	scanf("%d", &value);
	value2 = ~value;
	value2 += 1;

	printf("\"%d\"의 2의 보수(10진수): %d\n", value, value2);
	printf("\"%d\"의 2의 보수(16진수): %#x\n", value, value2);

	return 0;
}