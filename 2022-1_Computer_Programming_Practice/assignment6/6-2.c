//2번문제

#include<stdio.h>

int is_leap(int y);

int main() {
	int year;
	printf("연도를 입력하시오: ");
	scanf("%d", &year);
	int temp = is_leap(year);
	if (temp == 1)
		printf("%d년은 366일입니다.\n", year);
	else if (temp == 0)
		printf("%d년은 365일입니다.\n", year);


	return 0;
}

int is_leap(int y) {
	if (y % 4 == 0 && y % 100 != 0)
		return 1;

	else if (y % 400 == 0)
		return 1;

	else
		return 0;
}