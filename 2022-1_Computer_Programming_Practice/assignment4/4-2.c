// 2번문제
#include<stdio.h>

int main(void) {
	int height, age;

	printf("키를 입력하시오(cm): ");
	scanf("%d", &height);
	printf("나이를 입력하시오: ");
	scanf("%d", &age);

	if(height >= 140 && age >= 10)
		printf("탑승할 수 있습니다.\n");
	else
		printf("탑승할 수 없습니다.\n");

	return 0;
}