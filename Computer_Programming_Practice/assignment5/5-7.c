//7번 문제
#include<stdio.h>

int main() {
	double r, result;
	int n;
	printf("실수를 입력하시오: ");
	scanf("%lf", &r);
	result = r;
	printf("거듭제곱 횟수를 입력하시오: ");
	scanf("%d", &n);
	for (int i = 0; i < n-1; i++) {
		result *= r;
	}
	printf("결과값은 %f\n", result);
	

	return 0;
}