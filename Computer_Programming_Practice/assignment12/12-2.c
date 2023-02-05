//2¹ø¹®Á¦
#include <stdio.h>

#define ARRAY_INT(array, size, value) for(int i = 0; i < size; i++) array[i] = value;

int main() {
	int a[10];
	int size = sizeof(a) / sizeof(int);
	ARRAY_INT(a, size, 1);

	printf("[");
	for (int i = 0; i < size; i++) {
		printf("%d", a[i]);
		if (i == size - 1) break;
		else printf(", ");
	}
	printf("]\n");

	return 0;
}