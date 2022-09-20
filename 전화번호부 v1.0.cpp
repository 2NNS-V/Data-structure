// 전화번호부 v1.0

#if 0
#define CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <string.h>
#define MAX 100
#define BUFFER_SIZE 100

char* names[MAX];
char* numbers[MAX];
int n = 0;

void add();
void find();
void status();
void remove();

int main() {
	char com[BUFFER_SIZE];
	while (1) {
		printf("$");
		scanf("%s", com);

		if (strcmp(com, "add") == 0) add();
		else if (strcmp(com, "find") == 0) find();
		else if (strcmp(com, "status") == 0) status();
		else if (strcmp(com, "delete") == 0) remove();
		else if (strcmp(com, "exit") == 0) break;
	}
	return 0;	
}


void add() {
	char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
	scanf("%s", buf1);
	scanf("%s", buf2);
	names[n] = strdup(buf1);
	numbers[n] = strdup(buf2);
	n++;

	printf("%s was added successfully.\n", buf1);
}

void find() {
	char buf[BUFFER_SIZE];
	scanf("%s", buf);
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (strcmp(buf, names[i]) == 0) {
			printf("%s", numbers[i]);
			return;
		}
	}
	printf("No person named '%s' exists.\n", buf);
}

void status() {
	for (int i = 0; i < n; i++)
		printf("%s %s", names[i], numbers[i]);
	printf("Total %d persons.\n", n);
}

void remove() {
	char buf[BUFFER_SIZE];
	scanf("%s", buf);
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (strcmp(buf, names[i]) == 0) {
			names[i] = names[n - 1]; // 마지막 사람을 삭제된 자리로 옮김.
			numbers[i] = numbers[n - 1];
			n--;
			printf("%s was deleted successfully. \n", buf);
			return;
		}
	}
	printf("No person named '%s' exists.\n", buf);

}

#endif