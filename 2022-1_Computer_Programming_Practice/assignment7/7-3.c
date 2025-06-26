//3번문제

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_score(int score[][3]);
void find_test_stat(int score[][3]);

int main() {
	int score[10][3];
	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 3; j++) {
			score[i][j] = rand() % 100 + 1;
		}
	}
	print_score(score);
	find_test_stat(score);

	return 0;
}


void print_score(int score[][3]) {
	for (int i = 0; i < 10; i++) {
		printf("학생 %d의 성적: ", i + 1);
		for (int j = 0; j < 3; j++) {
			printf("%d	", score[i][j]);
		}
		printf("\n");
	}

}

void find_test_stat(int score[][3]) {
	int max, min;
	for (int i = 0; i < 3; i++) {
		max = 0;
		min = 1000;
		printf("시험 %d의 최고 점수 = ", i);
		for (int j = 0; j < 10; j++) {
			if (score[j][i] > max) {
				max = score[j][i];
			}
		}
		printf("%d\n", max);
		printf("시험 %d의 최저 점수 = ", i);
		for (int j = 0; j < 10; j++) {
			if (score[j][i] < min) {
				min = score[j][i];
			}
		}
		printf("%d\n", min);
	}
}