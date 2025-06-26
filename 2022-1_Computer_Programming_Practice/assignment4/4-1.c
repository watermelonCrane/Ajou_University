// 1번문제
#include<stdio.h>

int main(void) {
	int x, y, z;
	int min;	// 최소값
	printf("3개의 정수를 입력하시오: ");
	scanf("%d %d %d", &x, &y, &z);
	if (x < y) {
		if (x < z)
			min = x;
		else
			min = z;
	}
	else {
		if (y < z)
			min = y;
		else
			min = z;
	}
	printf("최소값은 %d입니다.\n", min);

	return 0;
}