//4번문제
#include <stdio.h>

#define GET_BIT(n, pos) (((n) >> (pos)) & 0x01)

void display_bit(int value) {
	for (int i = 31; i >= 0; i--) {
		printf("%d", GET_BIT(value, i));
	}
	printf("\n");
}

int main() {
	int n;
	int shift, number;
	printf("정수값을 입력하시오: ");
	scanf("%d", &n);
	printf("왼쪽 이동은 0, 오른쪽 이동은 1을 입력하시오: ");
	scanf("%d", &shift);
	printf("몇 비트 이동시킬 것인지 입력하시오: ");
	scanf("%d", &number);
	printf("이동 전: ");
	display_bit(n);
	printf("이동 후: ");
	if (shift == 0) {
		n = n << number;
	}
	else {
		n = n >> number;
	}
	display_bit(n);

	return 0;
}