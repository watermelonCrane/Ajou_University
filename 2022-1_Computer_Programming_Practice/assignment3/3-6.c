// 6번 문제
#include <stdio.h>

int main(void) {
	char c1, c2, c3, c4;
	printf("첫번째 문자를 입력하시오: ");
	scanf(" %c", &c1);
	printf("두번째 문자를 입력하시오: ");
	scanf(" %c", &c2);
	printf("세번째 문자를 입력하시오: ");
	scanf(" %c", &c3);
	printf("네번째 문자를 입력하시오: ");
	scanf(" %c", &c4);

	unsigned int value = 0;
	
	value |= c1;
	
	value |= c2 << 8;

	value |= c3 << 16;
	
	value |= c4 << 24;
	
	printf("결과 값(16진수): %x\n", value);
	

	return 0;
}