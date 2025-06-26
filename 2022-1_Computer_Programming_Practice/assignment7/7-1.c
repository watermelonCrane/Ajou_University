//1번문제

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RMAX 32768


int main() {
	int list[10], max = 0, min = RMAX;
	
	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		list[i] = rand() % RMAX;
		printf("%d ", list[i]);
	}
	printf("\n");
	for (int i = 0; i < 10; i++) {
		if (list[i] > max)
			max = list[i];
		if (list[i] < min)
			min = list[i];
	}
	printf("최대값은 %d\n최소값은 %d\n", max, min);

	return 0;
}