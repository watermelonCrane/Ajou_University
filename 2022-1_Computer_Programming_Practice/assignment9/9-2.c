//2번 문제

#include <stdio.h>
#include <ctype.h>


#define SIZE 100

void flip_str(char* s);

int main() {
	char str[SIZE];
	printf("문자열을 입력: ");
	scanf("%[^\n]s", str);
	flip_str(str);
	printf("%s\n", str);
	return 0;
}

void flip_str(char* s) {
	int i = 0;
	while (*(s + i) != '\0') {
		if (islower(*(s + i)))
			*(s + i) = toupper(*(s + i));
		else if (isupper(*(s + i)))
			*(s + i) = tolower(*(s + i));
		i++;
	}
}