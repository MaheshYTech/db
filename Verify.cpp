#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <conio.h>

#include <iostream>

using namespace std;

//#include "example.h"
#include "mylib.h"

class EMP {
	public:
		char name[50];
		int  age;
		float salary;
private:
	float da;
	float hra;
	float cca;
	float deduct;
	float grossSalary;
	float netSalary;

public:
		EMP(char tname[] , int tage , float tsalary) {
			strcpy(this->name, tname);
			this->age = tage;
			this->salary = tsalary;
			this->deduct = 1000.00;
			this->calcAllowence();
			this->calcGrossSalary();
			this->calcNetSalary();

			//this->da = this->salary * 0.05;
			//this->hra = this->salary * 0.10;
			//this->cca = this->salary * 0.05;

			//this->grossSalary = this->salary + this->da + this->hra + this->cca;
			//this->netSalary = this->grossSalary - this->deduct;
		}

		EMP() {
			this->name[0] = NULL;
			this->age = 0;
			this->salary = 0;
			this->calcAllowence();
			this->calcGrossSalary();
			this->calcNetSalary();
		}

	public:
		void printEmployee() {
			cout << "Employee Name  :: " << this->name << endl;
			cout << "Employee Age  :: " << this->age << endl;
			cout << "Employee Salary  :: " << this->salary << endl;
			cout << "D.A :: " << this->da << endl;
			cout << "H.R.A :: " << this->hra << endl;
			cout << "CCA :: " << this->cca << endl;
			cout << "=================================================" << endl;
			cout << "Gross Salary  :: " <<  this->grossSalary << endl;
			cout << "-------------------------------------------------" << endl;
			cout << "Net Salary  :: " << this->netSalary << endl;
			cout << "=================================================" << endl;
		}

	private:
		void calcAllowence() {
			this->da = this->salary * 0.05;
			this->hra = this->salary * 0.10;
			this->cca = this->salary * 0.05;
		}

		void calcGrossSalary() {
			this->grossSalary = this->salary + this->da + this->hra + this->cca;
		}

		void calcNetSalary() {
			this->netSalary = this->grossSalary - this->deduct;
		}

};

char fileName[30] = { NULL };

void replaceChar(char* str, char orgChar , char replChar) {
	int i = 0, n = 0;
	char ch = ' ';

	while (*(str + i)) {
		ch = *(str + i);
		if (ch == orgChar) {
			*(str + i) = replChar;
		}
		i++;
	}


	*(str + i) = NULL;
}

void splitSeleToArr(char* str, char **strArr) {
	int i = 0, n = 0 , len = 0;
	char ch = ' ';
	char* prevPos = NULL;

	len = strlen(str);

	prevPos = str;
	while (*(str + i)) {
		ch = *(str + i);
		if (ch == ',') {
			//strArr[n] = prevPos;
			*(str + i) = NULL;
			prevPos = (str + i + 1);
			n++;
			i++;
		}
		i++;
	}

	//strArr[n] = prevPos;

	*strArr = str;

	*(str + i) = NULL;
}

void getFieldFromStr(char* srcStr) {
	char strField[30] = { NULL };

}


int storeLine(char *src , char *str_arr[] , int aindx) {
	char *dst = (char*)calloc(1, strlen(src) + 2);
	strcpy(dst, src);
	str_arr[aindx] = dst;
	
	return  ++aindx;
}

char* myMemReAlloc(char* src, int presSize,  int reSize) {
	int i = 0;
	char* temp = (char*)calloc(reSize, sizeof(char));
	while (i < presSize) {
		temp[i] = src[i];
		i++;
	}

	free(src);

	return temp;
}

void readAllLines(int fd) {
	int START_POS = 0;
	int bytes_read = 0;
	int ORG_BUFF_SIZE = 64;
	int BUFF_SIZE = ORG_BUFF_SIZE;
	int const READ_SIZE = 10;
	int arrIndx = 0;
	int LAST_INDX = 0;
	char* readBuff = NULL;
	char* found_char = NULL;
	char strBuff[256] = { NULL };
	char* strArr[100] = { NULL };
	bool NEW_LINE_FOUND = false , NULL_FOUND = false;

	
	readBuff = CHPTR calloc(1, BUFF_SIZE);
	//fd = open("stocks.csv", O_BINARY | O_RDONLY);
	memset(readBuff, NULL, BUFF_SIZE);
	START_POS = 0;


	while (1) {
		if (START_POS == 0) 
		{
			memset(readBuff, NULL, BUFF_SIZE);
		}
		START_POS = 0;

		bytes_read = read(fd, strBuff + START_POS, READ_SIZE);

		if (bytes_read <= 0) {
			if (LAST_INDX > BUFF_SIZE) {
				readBuff = myMemReAlloc(readBuff, BUFF_SIZE, LAST_INDX + 10);
				BUFF_SIZE = LAST_INDX + 10;
			}
			arrIndx = storeLine(readBuff, strArr, arrIndx);
			//BUFF_SIZE = ORG_BUFF_SIZE;
			break;
		}
		else if (bytes_read < READ_SIZE) {
			//printf("\n %s", readBuff);
		}

		
		found_char = strchr(strBuff, '\r');

		if (found_char != NULL) {
			int i = 0;
			while (1) {
				if (*(found_char + i) == NULL) {
					lseek(fd, (i-2) * -1, SEEK_CUR);
					break;
				}
				i++;
			}
			*found_char = NULL;
			LAST_INDX = LAST_INDX + bytes_read;
			if (LAST_INDX > BUFF_SIZE) {
				readBuff = myMemReAlloc(readBuff, BUFF_SIZE, LAST_INDX + 10);
				BUFF_SIZE = LAST_INDX + 10;
			}
			strcat(readBuff, strBuff);
			//printf("\n %s", readBuff);

			arrIndx = storeLine(readBuff, strArr, arrIndx);

			if (strncmp(readBuff, "ACRYSIL", 7) == 0) {
				START_POS = 0;
			}
			START_POS = 0;
			memset(readBuff, NULL, BUFF_SIZE);
			memset(strBuff, NULL, 256);
			BUFF_SIZE = ORG_BUFF_SIZE;
			readBuff = (char*)calloc(1, BUFF_SIZE);
			LAST_INDX = 0;
		}else {
			LAST_INDX = LAST_INDX + bytes_read;
			if (LAST_INDX > BUFF_SIZE) {
				readBuff = myMemReAlloc(readBuff, BUFF_SIZE, LAST_INDX + 10);
				BUFF_SIZE = LAST_INDX + 10;
			}
			strcat(readBuff, strBuff);
			memset(strBuff, NULL, 256);
			START_POS = bytes_read;
		}
	}

	for (int i = 0; i < 100; i++) {
		if (strArr[i] == NULL) {
			break;
		}

		printf("\n %s", strArr[i]);
		free(strArr[i]);
	}
}


char *getLine(int fd, int *bytesRead) {
	int  START_POS = 0;
	int  bytes_read = 0;
	int  ORG_BUFF_SIZE = 64;
	int  BUFF_SIZE = ORG_BUFF_SIZE;
	int  const READ_SIZE = 50;
	int  arrIndx = 0;
	int  LAST_INDX = 0;
	char FILL_CHAR = 32;
	char *readBuff = NULL;
	char *found_char = NULL;
	char strBuff[256] = { NULL };
	char *strArr[100] = { NULL };
	bool NULL_FOUND = false;


	readBuff = CHPTR calloc(1, BUFF_SIZE);
	//fd = open("stocks.csv", O_BINARY | O_RDONLY);
	memset(readBuff, NULL, BUFF_SIZE);
	START_POS = 0;

	while (1) {
		if (START_POS == 0)
		{
			//memset(readBuff, NULL, BUFF_SIZE);
		}
		START_POS = 0;
		bytes_read = read(fd, strBuff + START_POS, READ_SIZE);
		if (bytes_read <= 0) {
			*bytesRead = 0;
			return readBuff;
		}
		else if (bytes_read < READ_SIZE) {
			//printf("\n %s", readBuff);
		}


		found_char = strchr(strBuff, '\r');

		if (found_char != NULL) {
			int i = 0;
			while (1) {
				if (*(found_char + i) == NULL) {
					lseek(fd, (i - 2) * -1, SEEK_CUR);
					break;
				}
				i++;
			}
			*found_char = NULL;
			LAST_INDX = LAST_INDX + bytes_read;
			if (LAST_INDX > BUFF_SIZE) {
				readBuff = myMemReAlloc(readBuff, BUFF_SIZE, LAST_INDX + 10);
				BUFF_SIZE = LAST_INDX + 10;
			}
			strcat(readBuff + LAST_INDX -bytes_read, strBuff);
			*bytesRead = bytes_read;
			break;
		}
		else {
			LAST_INDX = LAST_INDX + bytes_read;
			if (LAST_INDX > BUFF_SIZE) {
				readBuff = myMemReAlloc(readBuff, BUFF_SIZE, LAST_INDX + 10);
				BUFF_SIZE = LAST_INDX + 10;
			}
			strcat(readBuff + LAST_INDX - bytes_read, strBuff);
			memset(strBuff, NULL, 256);
			//START_POS = bytes_read;
		}
	}

	//printf("\n %s", readBuff);

	return readBuff;
}

char sql[255] = { "select symbol, volume,  nodays, dept from db.txt where nodays > 50 or dept = 'AUTO' order by symbol" };

void main() {
	char* charArr[5] = { NULL };
	int fd = 0;
	int bytesRead = 0;
	char msg1[100] = { "select symbol, volume,  nodays, dept from db.txt" };
	char msg2[100] = { "select symbol, volume,  nodays, dept from db.txt where nodays > 50" };
	char* readBuff = NULL;
	charArr[0] = msg2;

	//myMemCopy(msg1, msg2, strlen(msg1));

	//fd = open("stocks.csv", O_BINARY | O_RDONLY);
	//while (1) {

	//	readBuff = getLine(fd , &bytesRead);
	//	printf("\n %s", readBuff);
	//	if (bytesRead == 0) {
	//		break;
	//	}
	//}
	//close(fd);

	EMP emp =  EMP((char*)"Bill Gates", 64, 45000.00);
	emp.printEmployee();

	readBuff = (char*)calloc(1, sizeof(100));

	printf("\n\n %lu ", *((int*)readBuff - 4));

	free(readBuff);

	//strcpy(charArr[0], CHPTR "hello");
	//printf("\n %c", *charArr[0]);

	//getSelectFields(sql);

	
	//_readSystemCall();
	_getch();
}