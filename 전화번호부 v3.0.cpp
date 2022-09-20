// 전화번호부 v3.0

#if 0
#define CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define INIT_CAPACITY 3
#define BUFFER_SIZE 50

char** names; // 포인터
char** numbers;

int capacity = INIT_CAPACITY;
int n = 0;
char delim[] = " ";

void add();
void find();
void status();
void remove();
void save();
void load();

void init_directory() {
	names = (char**)malloc(INIT_CAPACITY * sizeof(char*)); // malloc(바이트수)
	numbers = (char**)malloc(INIT_CAPACITY * sizeof(char*));
}

void process_command() {
	char command_line[BUFFER_SIZE] ;
	char* com, * argu1, * argu2;

	while (1) {
		printf("$");
		if (readline(command_line, BUFFER_SIZE) <= 0) continue;
		com = strtok(command_line, delim);
		if (com == NULL) continue;
		
		if (strcmp(com, "add") == 0) {
			argu1 = strtok(NULL, delim); // 이름
			argu2 = strtok(NULL, delim); // 전화번호
			if (argu1 == NULL) {
				printf("Name required.\n");
				continue;
			}
			if (argu2 == NULL) {
				printf("Number required.\n");
				continue;
			}
			if (argu1 == NULL || argu2 == NULL) {
				printf("Invalid argumets.\n");
				continue;
			}
			add(argu1, argu2);
			printf("%s was added successfully.\n", argu1);
		}

		else if (strcmp(com, "find") == 0) {
			argu1 = strtok(NULL, delim);
			if (argu1 == NULL) {
				printf("Name required.\n");
				continue;
			}
			find(argu1);
		}
		else if (strcmp(com, "status") == 0) 
			status();
		else if (strcmp(com, "delete") == 0) {
			argu1 = strtok(NULL, delim);
			if (argu1 == NULL) {
				printf("Name required.\n");
				continue;
			}
			remove(argu1);
		}

		else if (strcmp(com, "read") == 0) {
			argu1 = strtok(NULL, delim); // 파일명
			if (argu1 == NULL) {
				printf("FileName required.\n");
				continue;
			}
			load(argu1);
		}

		else if (strcmp(com, "save") == 0) {
			argu1 = strtok(NULL, delim); // 파일명
			if (argu1 == NULL) {
				printf("FileName required.\n");
				continue;
			}
			save(argu1);
		}
		else if (strcmp(com, "exit") == 0) break;
	}
}

int readline(char str[], int lim) {
	int ch, i = 0;

	while ((ch = getchar()) != '\n')
		if (i < lim - 1)
			str[i++] = ch;
	str[i] = '\0';
	return i;
}

int main() {
	init_directory();
	process_command();
}

void reallocate() {
	capacity *= 2; // 크기가 2배인 배열 할당
	char** tmp1 = (char**)malloc(capacity * sizeof(char*));
	char** tmp2 = (char**)malloc(capacity * sizeof(char*));

	for (int i = 0; i < n; i++) {
		tmp1[i] = names[i];
		tmp2[i] = numbers[i];
	}

	free(names);
	free(numbers);

	// 배열이 늘어난 것과 같아짐.
	names = tmp1;
	numbers = tmp2;
}

void add(char* name, char* number) {
	if (n >= capacity) reallocate(); // 배열이 꽉 찬 경우

	int i = n - 1;
	while (i >= 0 && strcmp(names[i], name) > 0) {
		names[i + 1] = names[i];
		numbers[i + 1] = numbers[i];
		i--;
	}
	names[i + 1] = strdup(name);
	numbers[i + 1] = strdup(number);
	n++;

	printf("%s was added successfully.\n", name);
}


void load(char* filename) {
	char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];

	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("failed.\n"); // 파일 열기 오류
		return;
	}

	while ((fscanf(fp, "%s", buf1) != EOF)) {
		fscanf(fp, "%s", buf2);
		add(buf1, buf2);
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

void remove(char * name) {
	int index = search(name);
	if (index == -1) {
		printf("No person named '%s' exists.\n", name);
	}

	int j = index;
	for (; j < n - 1; j++) { // 한 칸씩 앞으로 당겨옴.
		names[j] = names[j + 1];
		numbers[j] = numbers[j + 1];
	}
	n--;
	printf("%s was deleted successfully. \n", name);
}

void find(char * name) {
	int index = search(name);
	if (index == -1)
		printf("No person named '%s' exists.\n", name);
	else printf("%s", numbers[index]);
}

void save(char * filename) {
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