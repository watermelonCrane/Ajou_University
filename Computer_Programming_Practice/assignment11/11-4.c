//4번 문제
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	FILE* fp1 = NULL, * fp2 = NULL;
	char fname1[100], fname2[100];

	printf("첫번째 파일: ");
	scanf("%s", fname1);
	fp1 = fopen(fname1, "w");
	if (fp1 == NULL) {
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}
	
	printf("두번째 파일: ");
	scanf("%s", fname2);
	fp2 = fopen(fname2, "wb");
	if (fp2 == NULL) {
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}

	srand(time(NULL));
	for (int i = 0; i < 100; i++) {
		int tmp = rand();
		printf("%d", tmp);
		fprintf(fp1, "%d", tmp);
		fwrite(&tmp, sizeof(int), 1, fp2);
	}

	fclose(fp1);
	fclose(fp2);

	int size;

	fp1 = fopen(fname1, "r");
	fseek(fp1, 0, SEEK_END);
	size = ftell(fp1);
	fclose(fp1);
	printf("\nThe size of %s: %d", fname1, size);

	fp2 = fopen(fname2, "r");
	fseek(fp2, 0, SEEK_END);
	size = ftell(fp2);
	fclose(fp2);
	printf("\nThe size of %s: %d\n", fname2, size);

	return 0;
}