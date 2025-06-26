/*
===과제 조건 추가===
Q1. 총 고객의 수는 미리 입력받은 상태인가요?

Q1-1. 만약 총 고객의 수를 모른다면 100번 반복하는 방법 말고 어떻게 몇 번만 반복하고 끝낼 수 있을 까요?

A1. 고객정보를 계속해서 입력받다가, end 입력받으면 고객 정보 입력이 끝났다고 생각합시다.
Q2. 최고 수영 실력과 골프 최저타 기록이 동일한 경우 누구의 이름과 기록을 출력해야하나요?
A2. 최고 기록이 두 명 이상일 경우, 가장 나중에 고객리스트에 올라간 사람의 기록을 출력하도록 합시다.
*/


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct Member {
	char name[21];
	char phone[20];
	enum code {
		S,
		G,
		Y
	} code;
	union {
		float sec;
		int score;
		bool spine;
	} info;
} Member;

void input_Member(Member* member, int* count);
int indexing_Member(Member* member, int* count, int** index);
void print_Member(Member* member, int* count);
void print_swim_info(Member* member, int count, int* index);
void print_golf_info(Member* member, int count, int* index);
void print_yoga_info(Member* member, int count, int* index);

int main() {

	Member member[99];
	int count[3] = { 0 };	// 0: swimming, 1: golf, 2: yoga | 각 종목의 개수 카운팅

	input_Member(member, count);

	int* index[3] = { 0 };	// 0: swimming, 1: golf, 2: yoga | 각각 동적배열로 할당받아 종목별 인덱스 정리
	int malloc_err = indexing_Member(member, count, index);
	if (malloc_err != 0) {
		printf("%d번째 동적할당 오류\n", malloc_err);
		exit(1);
	}
	
	print_Member(member, count);
	print_swim_info(member, count[0], index[0]);
	print_golf_info(member, count[1], index[1]);
	print_yoga_info(member, count[2], index[2]);

	for (int i = 0; i < 3; i++) {	//index 동적할당 해제
		free(index[i]);
	}

	return 0;
}

void print_yoga_info(Member* member, int count, int* index) {
	if (count == 0) return;

	int sum = 0;
	for (int i = 0; i < count; i++) {
		if (member[index[i]].info.spine == true) {
			sum++;
		}
	}

	printf("요가 척추질환 보유자: %d명\n", sum);
}

void print_golf_info(Member* member, int count, int* index) {
	if (count == 0) return;

	int sum = member[index[0]].info.score;
	int low = member[index[0]].info.score;
	int low_index = index[0];

	for (int i = 1; i < count; i++) {
		if (member[index[i]].info.score < low) {
			low = member[index[i]].info.score;
			low_index = index[i];
		}
		sum += member[index[i]].info.score;
	}

	printf("골프 최저타 기록 보유자: %s %d타\n", member[low_index].name, low);
	printf("골프 평균 타수: %d타\n", sum / count);
}

void print_swim_info(Member* member, int count, int* index) {
	if (count == 0) return;

	float sum = member[index[0]].info.sec;
	float best = member[index[0]].info.sec;
	int best_index = index[0];
	
	for (int i = 1; i < count; i++) {
		if (member[index[i]].info.sec < best) {
			best = member[index[i]].info.sec;
			best_index = index[i];
		}
		sum += member[index[i]].info.sec;
	}
	printf("최고 수영 실력자 : %s %.1f초\n", member[best_index].name, best);
	printf("수영 평균 기록: %.1f초\n", sum / count);

}

void print_Member(Member* member, int* count) {
	printf("수강생 인원: 골프 %d명, 수영 %d명, 요가 %d명\n", count[1], count[0], count[2]);
}

//고객정보가 저장된 배열에 대해서 종목별 배열 인덱스 넘버 저장. 다시말해 index포인터배열에 각 종목별 개수를 동적할당 후 인덱스 번호 저장
int indexing_Member(Member* member, int* count, int** index) {
	index[0] = (int*)malloc(sizeof(int) * count[0]);
	if (index[0] == NULL) return 1;		//malloc 오류 체크
	index[1] = (int*)malloc(sizeof(int) * count[1]);
	if (index[1] == NULL) return 2;
	index[2] = (int*)malloc(sizeof(int) * count[2]);
	if (index[2] == NULL) return 3;

	int recount[3] = { 0 };
	for (int i = 0; i < (count[0] + count[1] + count[2]); i++) {
		if (member[i].code == S) {
			index[0][recount[0]] = i;
			recount[0]++;
		}
		else if (member[i].code == G) {
			index[1][recount[1]] = i;
			recount[1]++;
		}
		else if (member[i].code == Y) {
			index[2][recount[2]] = i;
			recount[2]++;
		}
	}

	return 0;
}

//고객의 정보를 최대 99명까지 입력, end를 입력하면 입력루프 종료
void input_Member(Member* member, int* count) {
	printf("고객 정보를 입력해주세요 ([이름] [전화번호] [강습 코드] [추가 정보])\n");
	char tmp[21];
	for (int i = 0; i < 99; i++) {
		scanf("%s", tmp);
		if (strcmp(tmp, "end") == 0) break;
		else strcpy(member[i].name, tmp);
		scanf("%s", member[i].phone);
		scanf(" %c", &tmp[0]);
		if (tmp[0] == 'S') {
			member[i].code = S;
			count[0]++;
			scanf("%f", &member[i].info.sec);
		}
		else if (tmp[0] == 'G') {
			member[i].code = G;
			count[1]++;
			scanf("%d", &member[i].info.score);
		}
		else if (tmp[0] == 'Y') {
			member[i].code = Y;
			count[2]++;
			scanf("%s", tmp);
			if (strcmp(tmp, "TRUE") == 0) member[i].info.spine = true;
			else member[i].info.spine = false;
		}
	}
}