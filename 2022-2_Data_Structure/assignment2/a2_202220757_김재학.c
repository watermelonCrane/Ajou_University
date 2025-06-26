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
	q.back = q.front = 0;	//큐 초기화

	bool bell[5] = { 0 };	//진동벨 사용상태, false: 사용가능, true: 사용중. 0번부터 순서대로 101~105

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
			printf("서비스를 종료합니다.\n");
			loop = false;
			break;
			//return 0;
		default:
			printf("잘못 누르셨습니다.\n");
		}
		printf("*_*_*_*_*_*_*\n");
	}


	return 0;
}

void Complet(Queue* qq, bool* bell) {
	if (IsEmpty(qq)) {
		printf("주문 내역이 없습니다.\n");
		return;
	}
	int data = Pop(qq);
	bell[data - 101] = false;
	printf("%d번 님의 주문을 처리 중입니다.\n", data);
}

void Order(Queue* qq, bool* bell) {
	if (IsFull(qq)) {
		printf("주문이 불가능합니다.\n");
		printf("감사합니다.\n");
		return;
	}
	int data;
	while (1) {
		printf("현재 대기자 수는 %d명입니다. 계속 주문하시겠습니까?\n", Size(qq));
		printf("    1. 계속    2. 취소\n");
		scanf("%d", &data);
		if (data == 1) {
			while (1) {
				printf("진동벨의 번호를 입력해주세요.\n");
				Print_BellState(bell);
				scanf("%d", &data);
				if (data < 101 || data >105) {
					printf("올바르지 않은 번호입니다.\n");
					printf("*_*_*_*_*_*_*\n");
					continue;
				}
				if (Chek_Overlap(qq, data)) {
					printf("이미 사용중인 진동벨 입니다. 다른 번호를 입력해주세요\n");
					printf("*_*_*_*_*_*_*\n");
					continue;
				}
				Push(qq, data);
				bell[data - 101] = true;
				printf("주문이 완료되었습니다.\n");
				return;
			}
		}
		else if (data == 2) {
			printf("주문을 취소하셨습니다. 처음 화면으로 돌아갑니다.\n");
			break;
		}
		else {
			printf("잘못입력하셨습니다.\n");
		}
		printf("*_*_*_*_*_*_*\n");
	}

}

void Welcom() {
	printf("Ajou 푸드코트에 오신 것을 환영합니다. 원하는 기능을 선택해 주세요.\n");
	printf("    1. 주문    2. 서빙 완료    3. 종료\n");
}


//벨 사용상태 출력
void Print_BellState(bool* bell) {
	printf("벨 사용상태: ");
	for (int i = 0; i < 5; i++) {
		printf("[%d번: ", i + 101);
		if (bell[i] == true) {
			printf("사용중] ");
		}
		else {
			printf("사용가능] ");
		}
	}
	printf("\n");
}

//중복된 데이터값이면 true을 반환, 아니면 false을 반환
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
