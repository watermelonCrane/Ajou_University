#include <stdio.h>
#include <stdlib.h>

typedef struct addr{
	char name[100];
	char phone[100];
} ADDR;

int main() {
	ADDR* addrs = NULL;
	int amount;

	printf("주소의 개수: ");
	scanf("%d", &amount);

	addrs = (ADDR*)malloc(amount * sizeof(ADDR));
	if (addrs == NULL) {
		printf("동적 메모리 할당 오류\n");
		exit(1);
	}

	for (int i = 0; i < amount; i++) {
		printf("이름을 입력하시오: ");
		scanf("%s", &addrs[i].name);
		printf("휴대폰 번호를 입력하시오: ");
		scanf("%s", &addrs[i].phone);
	}
	printf("==========================\n");
	printf("이름	휴대폰 번호\n");
	printf("==========================\n");
	for (int i = 0; i < amount; i++) {
		printf("%s	%s\n", addrs[i].name, addrs[i].phone);
	}
	printf("==========================\n");
	free(addrs);

	return 0;
}