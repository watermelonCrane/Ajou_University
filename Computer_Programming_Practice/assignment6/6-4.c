//4¹ø¹®Á¦

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double f_rand();

int main() {
	
	srand((unsigned)time(NULL));
	for (int i = 0; i < 5; i++) {
		printf("%f ", f_rand());
	}
	

	return 0;
}

double f_rand() {

	
	return (double)rand() / RAND_MAX;
}