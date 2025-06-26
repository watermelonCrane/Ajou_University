#include <stdio.h>
#include <stdlib.h>

int main() {
	int amount;
	int* list = NULL;

	printf("정수의 개수: ");
	scanf("%d", &amount);

	list = (int*)malloc(amount * sizeof(int));
	if (list == NULL) {
		printf("동적 메모리 할당 오류\n");
		exit(1);
	}

	for (int i = 0; i < amount; i++) {
		int tmp;
		printf("양의 정수를 입력하시오: ");
		scanf("%d", &list[i]);
	}
	
	int sum = 0;

	printf("입력된 정수: ");
	for (int i = 0; i < amount; i++) {
		printf("%d ", list[i]);
		sum += list[i];
	}

	printf("\n입력된 정수들의 합은 %d입니다.\n", sum);

	free(list);

	return 0;
}