//6번문제
#include<stdio.h>

int main() {
	int x, n = 0, sum = 0;
	printf("정수를 입력하시오: ");
	scanf("%d", &x);

	while (sum < x) {
		n++;
		sum += n;
		if (sum + n + 1 >= x) {
			printf("1부터 %d까지의 합이 %d입니다.\n", n, sum);
			break;
		}
	}
	return 0;
}