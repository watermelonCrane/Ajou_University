//6번문제

#include<stdio.h>

int sum(int num);

int main() {
	int n;
	printf("정수를 입력하시오: ");
	scanf("%d", &n);

	printf("1부터 %d까지의 합=%d\n", n, sum(n));
	
	return 0;
}

int sum(int num) {
	
	if (num <= 1)
		return 1;
	else
		return num + sum(num - 1);
	

}