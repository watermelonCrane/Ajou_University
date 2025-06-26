//3번문제

#include <stdio.h>
#include <string.h>

int main() {
	char s[200];					//문자열
	char find[30];					//찾을 단어
	char replace[30];				//바꿀 단어
	char target[200] = { '\0' };	//변환된 문자열
	

	printf("문자열을 입력: ");
	scanf(" %[^\n]s", s);
	printf("찾을 단어: ");
	scanf(" %[^\n]s", find);
	printf("바꿀 단어: ");
	scanf(" %[^\n]s", replace);

	char seps[] = " ";
	
	
	char* token = strtok(s, seps);

	while (token != '\0') {
		if (strcmp(find, token) != 0) {
			strcat(target, token);
			strcat(target, " ");
		}
		else {
			strcat(target, replace);
			strcat(target, " ");
		}

		token = strtok(NULL, seps);
	}

	printf("변환된 문자열: %s\n", target);

	return 0;
}