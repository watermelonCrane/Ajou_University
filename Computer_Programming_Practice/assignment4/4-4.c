//4번 문제
#include<stdio.h>

int main(void) {
	double x, fx;

	printf("x값을 입력하시오: ");
	scanf("%lf", &x);
	
	if (x <= 0)
		fx = x * x - 8 * x + 1;
	else
		fx = 5 * x + 3;
	printf("%f\n", fx);

	return 0;
}