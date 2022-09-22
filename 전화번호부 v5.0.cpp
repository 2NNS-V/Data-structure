// 전화번호부 v5.0

#if 1
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

Person ** directory; // 구조체의 포인터 배열 -> 주소값만 저장되기 때문에 복잡한 과정 생략됨
int capacity, n;
char delim[] = " ";

void init() {
	directory = (Person**)malloc(CAPACITY * sizeof(Person*));
	capacity = CAPACITY;
	n = 0;
}

int readline(FILE* fp, char str[], int lim) {
	int ch, i = 0;

	while ((ch = fgetc(fp)) != '\n' && ch != EOF)
		if (i < lim)
			str[i++] = ch;
	str[i] = '\0';
	return i;
}

void reallocate() {
	capacity *= 2;
	Person** tmp = (Person**)malloc(capacity * sizeof(Person*));

	for (int i = 0; i < n; i++)
		tmp[i] = directory[i]; // 주소 복사
	free(directory);
	directory = tmp; // directory는 쓸모 없는 배열이 됨
}

void add(char* name, char* number, char* email, char* group) {
	if (n >= capacity) 
		reallocate();

	int i = n - 1;
	while (i >= 0 && strcmp(directory[i]->name, name) > 0) { // 한칸씩 뒤로 미룬다.
		directory[i + 1] = directory[i]; // 포인터 주소가 복사되는 것임
		i--;
	}
	directory[i + 1] = (Person*)malloc(sizeof(Person)); // 구조체 할당
	directory[i + 1]->name = name;
	directory[i + 1]->number = number;
	directory[i + 1]->email = email;
	directory[i + 1]->group = group;

	n++;
}


void load(char* filename) {
	char buffer[BUFFER_LENGTH];
	char* name, * number, * email, * group;
	char* token;

	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("failed.\n"); // 파일 열기 오류
		return;
	}

	while (1) {
		if (readline(fp, buffer, BUFFER_LENGTH) <= 0)
			break;
		name = strtok(buffer, "#");
		token = strtok(NULL, "#");
		if (strcmp(token, delim) == 0) number = NULL;
		else number = strdup(token);

		token = strtok(NULL, "#");
		if (strcmp(token, delim) == 0) email = NULL;
		else email = strdup(token);

		token = strtok(NULL, "#");
		if (strcmp(token, delim) == 0) group = NULL;
		else group = strdup(token);
		add(strdup(name), number, email, group);
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
		if (strcmp(name, directory[i]->name) != 0) return -1;
		else return i;
	}
}

void print_person(Person *p) {
	printf("%s:\n", p->name); // (*p).name 과 같음
	printf("	Phone: $s\n", p->number);
	printf("	Email: %s:\n", p->email);
	printf("	Group: %s:\n", p->group);
}

void release_person(Person *p) {
	free(p->name);
	if (p->number != NULL) free(p->number); // 데이터가 null인 경우는 제외
	if (p->email != NULL) free(p->email);
	if (p->group != NULL) free(p->group);
	free(p);

}

void remove(char* name) {
	int index = search(name);
	if (index == -1) {
		printf("No person named '%s' exists.\n", name);
		return;
	}

	Person* p = directory[index];
	for (int j =index; j < n - 1; j++) { // 한 칸씩 앞으로 당겨옴.
		directory[j] = directory[j + 1];
	}
	n--;
	release_person(p); // free(p)를 하지 않은 이유 : 모두 동적 메모리 할당임, 
	                   // p 뿐만 아니라 name, number 등도 모두 free 해야해서 함수를 따로 만듦
					   // strdup() 으로 할당받은 것들을 까먹기 쉬움
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
		fprintf(fp, "%s#", directory[i]->name);
		if  (directory[i]->number != NULL) fprintf(fp, "%s#", directory[i]->number);
		if (directory[i]->email!= NULL) fprintf(fp, "%s#", directory[i]->email);
		if (directory[i]->group != NULL) fprintf(fp, "%s#\n", directory[i]->group);
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
		if ((len + strlen(ptr) + 1) < lim) {
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
	add(name, (char*)(strlen(number) > 0 ? strdup(number) : empty),
		(char*)(strlen(email) > 0 ? strdup(email) : empty),
		(char*)(strlen(group) > 0 ? strdup(group) : empty));
}


int main() {
	char command_line[BUFFER_LENGTH];
	char* com, * argu;
	char name_str[BUFFER_LENGTH];

	init();
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