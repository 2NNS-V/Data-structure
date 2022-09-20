// 전화번호부 v2.0

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
void save();
void load();


int main() {
	char com[BUFFER_SIZE];
	while (1) {
		printf("$");
		scanf("%s", com);

		if (strcmp(com, "add") == 0) add();
		else if (strcmp(com, "find") == 0) find();
		else if (strcmp(com, "status") == 0) status();
		else if (strcmp(com, "delete") == 0) remove();
		else if (strcmp(com, "read") == 0) load();
		else if (strcmp(com, "save") == 0) save();
		else if (strcmp(com, "exit") == 0) break;
	}
	return 0;
}


void add() {
	char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
	scanf("%s", buf1);
	scanf("%s", buf2);

	int i = n - 1;
	while (i >= 0 && strcmp(names[i], buf1) > 0) {
		names[i + 1] = names[i];
		numbers[i + 1] = numbers[i];
		i--;
	}
	names[i+1] = strdup(buf1);
	numbers[i+1] = strdup(buf2);
	n++;

	printf("%s was added successfully.\n", buf1);
}


void load() { 
	char filename[BUFFER_SIZE];
	char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
	scanf("%s", filename);
	
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("failed.\n"); // 파일 열기 오류
		return;
	}

	while ((fscanf(fp, "%s", buf1) != EOF)) {
		fscanf(fp, "%s", buf2);
		names[n] = strdup(buf1);
		numbers[n] = strdup(buf2);
		n++;
	}
	fclose(fp);

}

void status() {
	for (int i = 0; i < n; i++)
		printf("%s %s", names[i], numbers[i]);
	printf("Total %d persons.\n", n);
}

int search(char* name) {
	for (int i = 0; i < n; i++) {
		if (strcmp(name, names[i]) != 0) return -1;
		else return i;
	}
}

void remove() {
	char buf[BUFFER_SIZE];
	scanf("%s", buf);

	int index = search(buf);
	if (index == -1) {
		printf("No person named '%s' exists.\n", buf);
	}

	int j = index;
	for (; j < n - 1; j++) { // 한 칸씩 앞으로 당겨옴.
		names[j] = names[j + 1];
		numbers[j] = numbers[j + 1];
	}
	n--;
	printf("%s was deleted successfully. \n", buf);
}

void find() {
	char buf[BUFFER_SIZE];
	scanf("%s", buf);
	int index = search(buf);
	if (index == -1)
		printf("No person named '%s' exists.\n", buf);
	else printf("%s", numbers[index]);
}

void save() {
	char filename[BUFFER_SIZE];
	scanf("%s", filename);

	FILE* fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("failed.\n");
		return;
	}
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%s %s\n", names[i], numbers[i]);
	}
	fclose(fp);
}


#endif