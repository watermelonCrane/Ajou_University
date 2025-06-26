/*
===���� ���� �߰�===
Q1. �� ���� ���� �̸� �Է¹��� �����ΰ���?

Q1-1. ���� �� ���� ���� �𸥴ٸ� 100�� �ݺ��ϴ� ��� ���� ��� �� ���� �ݺ��ϰ� ���� �� ���� ���?

A1. �������� ����ؼ� �Է¹޴ٰ�, end �Է¹����� �� ���� �Է��� �����ٰ� �����սô�.
Q2. �ְ� ���� �Ƿ°� ���� ����Ÿ ����� ������ ��� ������ �̸��� ����� ����ؾ��ϳ���?
A2. �ְ� ����� �� �� �̻��� ���, ���� ���߿� ������Ʈ�� �ö� ����� ����� ����ϵ��� �սô�.
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
	int count[3] = { 0 };	// 0: swimming, 1: golf, 2: yoga | �� ������ ���� ī����

	input_Member(member, count);

	int* index[3] = { 0 };	// 0: swimming, 1: golf, 2: yoga | ���� �����迭�� �Ҵ�޾� ���� �ε��� ����
	int malloc_err = indexing_Member(member, count, index);
	if (malloc_err != 0) {
		printf("%d��° �����Ҵ� ����\n", malloc_err);
		exit(1);
	}
	
	print_Member(member, count);
	print_swim_info(member, count[0], index[0]);
	print_golf_info(member, count[1], index[1]);
	print_yoga_info(member, count[2], index[2]);

	for (int i = 0; i < 3; i++) {	//index �����Ҵ� ����
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

	printf("�䰡 ô����ȯ ������: %d��\n", sum);
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

	printf("���� ����Ÿ ��� ������: %s %dŸ\n", member[low_index].name, low);
	printf("���� ��� Ÿ��: %dŸ\n", sum / count);
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
	printf("�ְ� ���� �Ƿ��� : %s %.1f��\n", member[best_index].name, best);
	printf("���� ��� ���: %.1f��\n", sum / count);

}

void print_Member(Member* member, int* count) {
	printf("������ �ο�: ���� %d��, ���� %d��, �䰡 %d��\n", count[1], count[0], count[2]);
}

//�������� ����� �迭�� ���ؼ� ���� �迭 �ε��� �ѹ� ����. �ٽø��� index�����͹迭�� �� ���� ������ �����Ҵ� �� �ε��� ��ȣ ����
int indexing_Member(Member* member, int* count, int** index) {
	index[0] = (int*)malloc(sizeof(int) * count[0]);
	if (index[0] == NULL) return 1;		//malloc ���� üũ
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

//���� ������ �ִ� 99����� �Է�, end�� �Է��ϸ� �Է·��� ����
void input_Member(Member* member, int* count) {
	printf("�� ������ �Է����ּ��� ([�̸�] [��ȭ��ȣ] [���� �ڵ�] [�߰� ����])\n");
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