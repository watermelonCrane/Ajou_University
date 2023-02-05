//2번문제
#include<stdio.h>

int main() {
	int a;
	printf("정수를 입력하시오: ");
	scanf("%d", &a);
	printf("%d의 약수: ", a);
	for (int i = 1; i <= a; i++) {
		if (a % i == 0)
			printf("%d ", i);
	}
	printf("\n");

	return 0;
}