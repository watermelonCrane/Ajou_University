//1번문제

#include<stdio.h>

int check_alpha(char c);

int main() {
	char c;
	int temp;
	printf("문자를 입력하시오: ");
	scanf("%c", &c);
	temp = check_alpha(c);

	if (temp == 1) {
		printf("%c는 알파벳 문자입니다.\n", c);
	}
	else if (temp == 0) {
		printf("%c는 알파벳 문자가 아닙니다.\n", c);
	}

	return 0;
}

int check_alpha(char c) {
	if (c >= 65 && c <= 90)
		return 1;
	else if (c >= 97 && c <= 122)
		return 1;
	else
		return 0;

}