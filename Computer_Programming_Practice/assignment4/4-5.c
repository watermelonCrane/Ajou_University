//5번문제
#include<stdio.h>

int main(void) {
	int x, y;
	printf("좌표를 입력하시오(x y): ");
	scanf("%d %d", &x, &y);

	int coor;	//사분면
	if (x > 0) {
		if (y > 0) {
			coor = 1;
		}
		if (y < 0) {
			coor = 4;
		}
	}
	if (x < 0) {
		if (y > 0) {
			coor = 2;
		}
		if (y < 0) {
			coor = 3;
		}
	}

	printf("(%d, %d)은 %d사분면\n", x, y, coor);

	return 0;
}