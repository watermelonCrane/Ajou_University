//4번문제

#include <stdio.h>
#include <string.h>

int main() {
	char s[100];
	int x, y;
	char op[10] = { '\0' };

	printf("수학 연산을 입력: ");
	scanf("%[^\n]s", s);

	char* token = strtok(s," ");
	strcat(op, token);
	token = strtok(NULL, " ");
	sscanf(token, "%d", &x);
	token = strtok(NULL, " ");
	sscanf(token, "%d", &y);

	if (strcmp(op, "add") == 0) {
		printf("연산의 결과: %d\n", x + y);
	}
	else if (strcmp(op, "sub") == 0) {
		printf("연산의 결과: %d\n", x - y);
	}
	else if (strcmp(op, "mult") == 0) {
		printf("연산의 결과: %d\n", x * y);
	}
	else if (strcmp(op, "div") == 0) {
		printf("연산의 결과: %d\n", x / y);
	}

	return 0;
}