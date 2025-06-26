#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_STACK_SIZE 10

//array stack 구현
typedef struct {
	int data[MAX_STACK_SIZE];
	int top;
}STACK;

STACK* creatStack();
bool isFull(STACK* stack);
bool isEmpty(STACK* stack);
void Push(STACK* stack, int item);
int Pop(STACK* stack);
void deleteStack(STACK* stack);


//linked stack 구현
typedef struct NODE{
	int data;
	struct NODE* link;
}NODE;

typedef struct {
	NODE* top;
	int count;
}StackList;

StackList* newStackList();
void initList(StackList* stack);
bool isEmptyList(StackList* stack);
bool isFullList(StackList* stack);
void pushList(StackList* stack, int item);
int popList(StackList* stack);
void deleteList(StackList* stack);


int main() {

	//array stack test
	STACK* arrStack = creatStack();
	int i = 0;
	for (i = 0; i < 11; i++) {
		Push(arrStack, i);
	}

	for (i = 0; i < 11; i++) {
		Pop(arrStack);
	}

	deleteStack(arrStack);

	//linked stack test
	StackList* linkedStack = newStackList();
	for (i = 0; i < 11; i++) {
		pushList(linkedStack, i);
	}

	for (i = 0; i < 11; i++) {
		popList(linkedStack);
	}

	deleteList(linkedStack);


	return 0;
}

StackList* newStackList() {
	StackList* temp = (StackList*)malloc(sizeof(StackList));
	
	initList(temp);
	return temp;
}

void initList(StackList* stack) {
	stack->top = NULL;
	stack->count = 0;
}

bool isEmptyList(StackList* stack) {
	return (stack->top == NULL);
}

bool isFullList(StackList* stack) {
	return (stack->count == MAX_STACK_SIZE);
}

void pushList(StackList* stack, int item) {
	if (isFullList(stack) == true) {
		fprintf(stderr, "스택이 최대크기입니다. (최대 스택 사이즈:%d)\n", MAX_STACK_SIZE);
		return;
	}
	NODE* temp = (NODE*)malloc(sizeof(NODE));
	temp->data = item;
	if (stack->top == NULL) {
		stack->top = temp;
		temp->link = NULL;
	}
	else {
		temp->link = stack->top;
		stack->top = temp;
	}
	stack->count++;

	//stack의 모든 데이터와 메모리 주소 출력
	printf("linked stack: \n");
	temp = stack->top;
	do {
		printf("%d %p\n", temp->data, &(temp->data));
		temp = temp->link;
	} while (temp != NULL);
}

int popList(StackList* stack) {
	if (isEmptyList(stack) == true) {
		fprintf(stderr, "스택이 비었습니다.\n");
		return -1;
	}
	else {
		int tmp = stack->top->data;
		if (stack->top->link == NULL) {
			free(stack->top);
			stack->top = NULL;
		}
		else {
			NODE* tmpNode = stack->top->link;
			free(stack->top);
			stack->top = tmpNode;
		}

		stack->count--;
		return tmp;
	}
}

void deleteList(StackList* stack) {
	if (isEmpty(stack) == true) {
		free(stack);
	}
	else {
		for (NODE* tmpNode = NULL; isEmpty(stack) == false;) {
			if (stack->top->link == NULL) {
				free(stack->top);
				stack->top = NULL;
			}
			else {
				tmpNode = stack->top->link;
				free(stack->top);
				stack->top = tmpNode;
			}
		}
		free(stack);
	}
}






STACK* creatStack() {
	STACK* stack = (STACK*)malloc(sizeof(STACK));
	
	stack->top = 0;
	return stack;
}


//꽉 찼으면 true리턴, 아니면 false리턴
bool isFull(STACK* stack) {
	return (stack->top == MAX_STACK_SIZE);
}

//비었으면 true리턴, 아니면 false 리턴
bool isEmpty(STACK* stack) {
	return (stack->top == 0);
}

void Push(STACK* stack, int item) {
	if (isFull(stack) == true) {
		fprintf(stderr, "Stack is full, cannot  add element\n");
		return;
	}
	else {
		stack->data[stack->top] = item;
		stack->top++;
	}
	
	//stack의 모든 원소의 값과 메모리 주소 출력
	printf("stack: \n");
	for (int select = stack->top - 1; select >= 0; select--) {
		printf("%d %p\n", stack->data[select], &(stack->data[select]));
	}
}

int Pop(STACK* stack) {
	if (isEmpty(stack) == true) {
		fprintf(stderr, "Stack is empty, cannot delet element\n");
		return -1;
	}
	else {
		stack->top--;
		return stack->data[stack->top];
	}
}

void deleteStack(STACK* stack) {
	free(stack);
}