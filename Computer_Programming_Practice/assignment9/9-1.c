//1번 문제

#include <stdio.h>
#include <string.h>

#define SIZE 100

void change_space(char s[]);

int main() {
	char str[SIZE];
	printf("문자열을 입력: ");
	scanf("%[^\n]s", str);
	change_space(str);
	printf("%s\n", str);

	return 0;
}

void change_space(char s[]) {
	char tmp[SIZE];
	for (int i = 0; i < strlen(s); i++) {
		if (s[i] == ' ')
			tmp[i] = ',';
		else
			tmp[i] = s[i];
	}
	for (int i = 0; i < strlen(s); i++) {
		s[i] = tmp[i];
	}
}

