// 1번 문제
#include <stdio.h>

int main(void) {
	
	int x, y, z, max;
	printf("3개의 정수를 입력하시오: ");
	scanf("%d %d %d", &x, &y, &z);

	(x > y) ? (max = x) : (max = y);
	(z > max) ? (max = z) : (max = max);

	printf("최대값: %d\n", max);

	return 0;
}