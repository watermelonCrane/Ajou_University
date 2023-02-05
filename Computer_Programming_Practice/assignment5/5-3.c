//3번문제
#include<stdio.h>

int main() {
	int num = 7;
	for (int i = 0; i < num; i++) {
		for (int k = i; k > 0; k--) {
			printf(" ");
		}
		for (int j = num; j > i; j--) {
			printf("*");
		}
		printf("\n");
	}
	return 0;
}