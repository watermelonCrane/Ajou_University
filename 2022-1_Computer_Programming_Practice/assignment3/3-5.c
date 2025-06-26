// 5번 문제
#include <stdio.h>

int main(void) {
	int x, y, result = 0;
	printf("x 좌표를 입력하시오: ");
	scanf("%d", &x);
	printf("y 좌표를 입력하시오: ");
	scanf("%d", &y);

	x > 0 && y > 0 ? (result = 1) : (result = result);
	x < 0 && y > 0 ? (result = 2) : (result = result);
	x < 0 && y < 0 ? (result = 3) : (result = result);
	x > 0 && y < 0 ? (result = 4) : (result = result);


	printf("(%d, %d)은 %d사분면\n", x, y, result);
	
	
	return 0;
}