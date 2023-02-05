// 4번 문제
#include <stdio.h>

int main(void) {
	int x, y;
	printf("2개의 정수를 입력하시오: ");
	scanf("%d %d", &x, &y);
	printf("%d << %d의 값: %d\n", x, y, x << y);
	printf("%d >> %d의 값: %d\n", x, y, x >> y);

	return 0;
}