//3번 문제

#include <stdio.h>
#include <stdlib.h>

#define SIZE 20

struct student {
	char name[100];
	double korean, english, math;
};

int main() {
	FILE* fp = NULL;
	char fname[100];
	struct student s;			//학생 성적
	double ksum = 0, esum = 0, msum = 0;	//전체 학생 과목별 합계

	printf("파일명을 입력하세요: ");
	scanf("%s", fname);

	fp = fopen(fname, "rb");

	if (fp == NULL) {
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}

	for (int i = 0; i < SIZE; i++) {
		fread(&s, sizeof(struct student), 1, fp);
		printf("이름 = %s, 국어 = %.1f, 영어 = %.1f, 수학 = %.1f\n", s.name, s.korean, s.english, s.math);
		ksum += s.korean;
		esum += s.english;
		msum += s.math;
	}
	fclose(fp);
	printf("평균: 국어= %.2f, 영어 = %.2f, 수학 = %.2f\n", ksum / SIZE, esum / SIZE, msum / SIZE);

	return 0;
}