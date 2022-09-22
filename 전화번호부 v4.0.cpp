// 전화번호부 v4.0

#if 0
#define CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define CAPACITY 100
#define BUFFER_LENGTH 100

typedef struct person {
	char* name;
	char* number;
	char* email;
	char* group;
} Person;

Person directory[CAPACITY];
int n = 0;
char delim[] = " ";


int readline(FILE * fp, char str[], int lim) {
	int ch, i = 0;

	while ((ch = fgetc(fp)) != '\n' && ch != EOF)
		if (i < lim)
			str[i++] = ch;
	str[i] = '\0';
	return i;
}


void add(char* name, char* number, char* email, char* group) {

	int i = n - 1;
	while (i >= 0 && strcmp(directory[i].name, name) > 0) {
		directory[i + 1] = directory[i];
		i--;
	}

	directory[i + 1].name = strdup(name);
	directory[i + 1].number = strdup(number);
	directory[i + 1].email = strdup(email);
	directory[i + 1].group = strdup(group);

	n++;
	printf("%s was added successfully.\n", name);
}


void load(char* filename) {
	char buffer[BUFFER_LENGTH];
	char* name, * number, * email, * group;

	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("failed.\n"); // 파일 열기 오류
		return;
	}

	while (1) {
		if (readline(fp, buffer, BUFFER_LENGTH) <= 0)
			break;
		name = strtok(buffer, "#");
		number = strtok(NULL, "#"); // 없는 경우엔 X
		email = strtok(NULL, "#");
		group = strtok(NULL, "#");
		add(name, number, email, group);
	}
	fclose(fp);

}

void status() {
	for (int i = 0; i < n; i++)
		print_person(directory[i]);
	printf("Total %d persons.\n", n);
}


int search(char* name) {
	for (int i = 0; i < n; i++) {
		if (strcmp(name, directory[i].name) != 0) return -1;
		else return i;
	}
}

void print_person(Person p) {
	printf("%s:\n", p.name);
	printf("	Phone: $s\n", p.number);
	printf("	Email: %s:\n", p.email);
	printf("	Group: %s:\n", p.group);
}

void remove(char* name) {
	int index = search(name);
	if (index == -1) {
		printf("No person named '%s' exists.\n", name);
	}

	int j = index;
	for (; j < n - 1; j++) { // 한 칸씩 앞으로 당겨옴.
		directory[j].name = directory[j + 1].name;
		directory[j].number = directory[j + 1].number;
		directory[j].email = directory[j + 1].email;
		directory[j].group = directory[j + 1].group;
	}
	n--;
	printf("%s was deleted successfully. \n", name);
}


void find(char* name) {
	int index = search(name);
	if (index == -1)
		printf("No person named '%s' exists.\n", name);
	else
		print_person(directory[index]);
}


void save(char* filename) {
	FILE* fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("Open failed.\n");
		return;
	}
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%s#", directory[i].name);
		fprintf(fp, "%s#", directory[i].number);
		fprintf(fp, "%s#", directory[i].email);
		fprintf(fp, "%s#\n", directory[i].group);
	}
	fclose(fp);
}


int compose_name(char str[], int lim) { // 2개 이상의 공백이 있을 때 공백을 제거
	char* ptr;
	int len = 0;

	ptr = strtok(NULL, delim);
	if (ptr == NULL) return 0;

	strcpy(str, ptr);
	len += strlen(ptr);

	while ((ptr = strtok(NULL, delim)) != NULL) {
		if ((len + strlen(ptr) + 1) < lim ) {
		str[len++] = ' ';
		str[len] = '\0';
		strcat(str, ptr);
		len += strlen(ptr);
		}
	}
	return len;
}

void handle_add(char* name) {
	char number[BUFFER_LENGTH], email[BUFFER_LENGTH], group[BUFFER_LENGTH];
	char empty[] = " ";

	printf("	Phone: ");
	readline(stdin, number, BUFFER_LENGTH);
	printf("	Email: ");
	readline(stdin, email, BUFFER_LENGTH);
	printf("	Group: ");
	readline(stdin, group, BUFFER_LENGTH);

	// a:b 입력한 경우 a, 입력하지 않은 경우 b
	add(name, (char*)(strlen(number) > 0 ? number : empty),
		(char*)(strlen(email) > 0 ? email : empty),
		(char*)(strlen(group) > 0 ? group : empty));
}


int main() {
	char command_line[BUFFER_LENGTH];
	char* com, * argu;
	char name_str[BUFFER_LENGTH];

	while (1) {
		printf("$");
		if (readline(stdin, command_line, BUFFER_LENGTH) <= 0)
			continue;
		com = strtok(command_line, delim);

		if (com == NULL) continue;
		if (strcmp(com, "add") == 0) {
			if (compose_name(name_str, BUFFER_LENGTH) <= 0) {
				printf("Name required.\n");
				continue;
			}
			handle_add(name_str);
		}


		else if (strcmp(com, "find") == 0) {
			if (compose_name(name_str, BUFFER_LENGTH) <= 0) {
				printf("Name required.\n");
				continue;
			}
			find(name_str);
		}

		else if (strcmp(com, "status") == 0)
			status();

		else if (strcmp(com, "delete") == 0) {
			if (compose_name(name_str, BUFFER_LENGTH) <= 0) {
				printf("Name required.\n");
				continue;
			}
			remove(name_str);
		}

		else if (strcmp(com, "read") == 0) {
			argu = strtok(NULL, delim); // 파일명
			if (argu == NULL) {
				printf("FileName required.\n");
				continue;
			}
			load(argu);
		}

		else if (strcmp(com, "save") == 0) {
			argu = strtok(NULL, delim); // 파일명
			if (argu == NULL) {
				printf("FileName required.\n");
				continue;
			}
			save(argu);
		}

		else if (strcmp(com, "exit") == 0) break;
	}
	return 0;
}
#endif