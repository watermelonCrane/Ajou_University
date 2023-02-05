//4번문제
#include <stdio.h>

void show_binary(int n);

int main() {
	int num;
	while (1) {
		printf("10진수를 입력하시오: ");
		scanf("%d", &num);
		if (num == -1)
			break;
		else
			show_binary(num);
	}
	return 0;
}

void show_binary(int n) {
	int binary[32] = { 0 }, temp = n, count = 0;
	for (int i = 31; i >= 0; i--) {
		binary[i] = temp % 2;
		temp /= 2;
		count++;
		if (temp == 0)
			break;
	}
	printf("%d의 2진수 표시: ", n);
	
	for (int i = (32 - count); i < 32; i++) {
		printf("%d", binary[i]);
	}


	printf("\n");
}