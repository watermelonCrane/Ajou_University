//6번문제

#include<stdio.h>

int main(void) {
	char c;
	printf("문자를 입력하시오: ");
	scanf("%c", &c);

	switch (c) {
		case 'C':
		case 'c':
			printf("Circle\n");
			break;
		case 'T':
		case 't':
			printf("Triangle\n");
			break;
		case 'R':
		case 'r':
			printf("Rectangle\n");
			break;
		default:
			printf("Unknown\n");
			break;
	}

	return 0;
}