#include <stdio.h>
#include <stdbool.h>
#define SIZE 6

typedef struct {
	int front, back;
	int data[SIZE];
} Queue;

bool IsFull(Queue* qq);
bool IsEmpty(Queue* qq);
void Push(Queue* qq, int data);
int Pop(Queue* qq);
int Size(Queue* qq);


void Print_BellState(bool* bell);

void Welcom();
void Order(Queue* qq, bool* bell);
void Complet(Queue* qq, bool* bell);
bool Chek_Overlap(Queue* qq, int data);

int main() {
	

	Queue q;
	q.back = q.front = 0;	//ť �ʱ�ȭ

	bool bell[5] = { 0 };	//������ ������, false: ��밡��, true: �����. 0������ ������� 101~105

	bool loop = true;
	int select;
	while (loop) {
		Welcom();
		scanf("%d", &select);
		switch (select) {
		case 1:
			Order(&q, bell);
			break;
		case 2:
			Complet(&q, bell);
			break;
		case 3:
			printf("���񽺸� �����մϴ�.\n");
			loop = false;
			break;
			//return 0;
		default:
			printf("�߸� �����̽��ϴ�.\n");
		}
		printf("*_*_*_*_*_*_*\n");
	}


	return 0;
}

void Complet(Queue* qq, bool* bell) {
	if (IsEmpty(qq)) {
		printf("�ֹ� ������ �����ϴ�.\n");
		return;
	}
	int data = Pop(qq);
	bell[data - 101] = false;
	printf("%d�� ���� �ֹ��� ó�� ���Դϴ�.\n", data);
}

void Order(Queue* qq, bool* bell) {
	if (IsFull(qq)) {
		printf("�ֹ��� �Ұ����մϴ�.\n");
		printf("�����մϴ�.\n");
		return;
	}
	int data;
	while (1) {
		printf("���� ����� ���� %d���Դϴ�. ��� �ֹ��Ͻðڽ��ϱ�?\n", Size(qq));
		printf("    1. ���    2. ���\n");
		scanf("%d", &data);
		if (data == 1) {
			while (1) {
				printf("�������� ��ȣ�� �Է����ּ���.\n");
				Print_BellState(bell);
				scanf("%d", &data);
				if (data < 101 || data >105) {
					printf("�ùٸ��� ���� ��ȣ�Դϴ�.\n");
					printf("*_*_*_*_*_*_*\n");
					continue;
				}
				if (Chek_Overlap(qq, data)) {
					printf("�̹� ������� ������ �Դϴ�. �ٸ� ��ȣ�� �Է����ּ���\n");
					printf("*_*_*_*_*_*_*\n");
					continue;
				}
				Push(qq, data);
				bell[data - 101] = true;
				printf("�ֹ��� �Ϸ�Ǿ����ϴ�.\n");
				return;
			}
		}
		else if (data == 2) {
			printf("�ֹ��� ����ϼ̽��ϴ�. ó�� ȭ������ ���ư��ϴ�.\n");
			break;
		}
		else {
			printf("�߸��Է��ϼ̽��ϴ�.\n");
		}
		printf("*_*_*_*_*_*_*\n");
	}

}

void Welcom() {
	printf("Ajou Ǫ����Ʈ�� ���� ���� ȯ���մϴ�. ���ϴ� ����� ������ �ּ���.\n");
	printf("    1. �ֹ�    2. ���� �Ϸ�    3. ����\n");
}


//�� ������ ���
void Print_BellState(bool* bell) {
	printf("�� ������: ");
	for (int i = 0; i < 5; i++) {
		printf("[%d��: ", i + 101);
		if (bell[i] == true) {
			printf("�����] ");
		}
		else {
			printf("��밡��] ");
		}
	}
	printf("\n");
}

//�ߺ��� �����Ͱ��̸� true�� ��ȯ, �ƴϸ� false�� ��ȯ
bool Chek_Overlap(Queue* qq, int data) {
	for (int pointer = qq->front; pointer != qq->back; pointer = (pointer + 1) % SIZE) {
		if (qq->data[(pointer + 1) % SIZE] == data) return true;
	}

	return false;
}

bool IsFull(Queue* qq) {
	return ((qq->back + 1) % SIZE == qq->front);
}

bool IsEmpty(Queue* qq) {
	return (qq->back == qq->front);
}

void Push(Queue* qq, int data) {
	qq->back = (qq->back + 1) % SIZE;
	qq->data[qq->back] = data;
}

int Pop(Queue* qq) {
	qq->front = (qq->front + 1) % SIZE;
	return qq->data[qq->front];
}

int Size(Queue* qq) {
	if (qq->back >= qq->front) return (qq->back - qq->front);
	else return (qq->back + SIZE - qq->front);
}
