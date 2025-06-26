// 1번 문제

#include<stdio.h>

int main() {
	int a, sum = 0, times = 1;
	printf("정수를 입력하시오: ");
	scanf("%d", &a);
	while (times <= a) {
		sum += times;
		times += 2;
	}
	printf("1부터 %d까지의 모든 홀수의 합은 %d입니다.\n", a, sum);

	return 0;
}