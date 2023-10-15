#pragma once

typedef struct FIELDS {
	char fname[30];
	char ftype;
	int fwidth;
}FIELDS;

FIELDS f = { "Name" , 'T' , 30 };

void StrToUpper(char *src);
void printField(FIELDS x);

void StrToUpper(char* src) {
	int i = 0;
	char ch = ' ';
	while (*(src + i)) {
		ch = *(src + i);
		if (ch >= 97 && ch <= 123) {
			*(src + i) = ch - 32;
		}
		i++;
	}
}

void printField(FIELDS x) {
	printf("\n %s %c %d", x.fname, x.ftype, x.fwidth);
}