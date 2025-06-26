//5번문제
#include<stdio.h>

int main() {
	int n;
	printf("정수를 입력하시오: ");
	scanf("%d", &n);
	printf("2부터 %d까지의 소수: ", n);
	for (int i = 2; i <= n; i++) {
		int sign = 1;
		for (int k = 2; k < i; k++) {
			if (i % k == 0) {
				sign = 0;
				break;
			}
			
		}
		if (sign)
			printf("%d ", i);
	}
	printf("\n");

	return 0;
}