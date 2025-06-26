//1번문제

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
	FILE* fp1 = NULL;
	FILE* fp2 = NULL;

	char fname1[100], fname2[100];

	printf("첫번째 파일: ");
	scanf("%s", fname1);

	fp1 = fopen(fname1, "r");
	if (fp1 == NULL) {
		printf("파일을 열수 없습니다.\n");
		exit(1);
	}

	printf("두번째 파일: ");
	scanf("%s", fname2);

	fp2 = fopen(fname2, "w");
	if (fp2 == NULL) {
		printf("파일을 열 수 없습니다.");
		exit(1);
	}

	int c;
	while ((c = fgetc(fp1)) != EOF) {
		if (islower(c) > 0) {
			c = toupper(c);
		}
		fputc(c, fp2);
	}

	fclose(fp2);
	fp2 = fopen(fname2, "r");
	while ((c = fgetc(fp2)) != EOF) {
		putchar(c);
	}

	fclose(fp1);
	fclose(fp2);
	
	return 0;
}