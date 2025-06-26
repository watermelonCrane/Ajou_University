#include <stdio.h>
#include <stdlib.h>

struct list {
	char str[100];
};

int main() {
	int amount;
	struct list *str = NULL;

	printf("문자열의 개수: ");
	scanf("%d", &amount);

	str = (struct list*)malloc(amount * sizeof(struct list));
	if (str == NULL) {
		printf("동적 메모리 할당 오류\n");
		exit(1);
	}

	for (int i = 0; i < amount; i++) {
		printf("문자열을 입력하시오: ");
		scanf("%s", &str[i].str);
	}


	printf("[");
	for (int i = 0; i < amount; i++) {
		printf("\"%s\"", str[i].str);

		if (i == amount - 1) {
			printf("]\n");
		}
		else {
			printf(" ");
		}
	}

	free(str);

	return 0;
}