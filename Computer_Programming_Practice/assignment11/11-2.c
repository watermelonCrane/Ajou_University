//2번문제

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	char fname1[100], fname2[100];
	char buffer1[1024], buffer2[1024];

	printf("첫번째 파일: ");
	scanf("%s", fname1);

	fp1 = fopen(fname1, "r");

	if (fp1 == NULL) {
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}

	printf("두번째 파일: ");
	scanf("%s", fname2);


	fp2 = fopen(fname2, "r");

	if (fp2 == NULL) {
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}

	while (fgets(buffer1, 1024, fp1) != NULL && fgets(buffer2, 1024, fp2) != NULL) {
		if (strcmp(buffer1, buffer2) != 0) {
			printf("<< %s", buffer1);
			printf(">> %s", buffer2);
		}
	}

	fclose(fp1);
	fclose(fp2);

	return 0;
}