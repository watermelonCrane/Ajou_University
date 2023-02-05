//3번문제

#include<stdio.h>

int main(void) {
	double height, weight;

	printf("키와 체중을 입력하시오: ");
	scanf("%lf %lf", &height, &weight);
	
	double std_weight = (height - 100) * 0.9;
	
	if (weight < std_weight)
		printf("저체중입니다.\n");
	else if (weight > std_weight)
		printf("과체중입니다.\n");
	else
		printf("표준체중입니다.\n");
	

	return 0;
}