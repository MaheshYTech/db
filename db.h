#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include "mylib.h"
#include "std_define.h"
#include <ctype.h>



typedef struct {
    char dbname[10];
    char dbversion[10];
    int  field_count;
    int  record_size;
    int  record_count;
    char ftype[32];
    int  fwidth[32];
    int  foffset[32];
}DB_MAIN;

DB_MAIN dbmain = { NULL, NULL, 0 , 0, 0, NULL , 0, 0 };

typedef struct FIELDS {
    char    fname[15];
    char    ftype;
    int     fwidth;
    int     decimal;
    int     foffset;
}FIELDS;

FIELDS Fields[MAX_FIELD] = { NULL };

typedef struct LISTS {
    LISTS* prev;
    LISTS* next;
    void* data;
}LISTS;

typedef struct MAIN {
    LISTS* start;
    LISTS* end;
};

MAIN* lmain = NULL;

typedef struct SELECTFIELDS {
    FIELDS Fields;
    void* data;
    //SELECT_FIELDS* next;
}SELECTFIELDS;

SELECTFIELDS* seleFields = NULL;

char srcData[256] = { NULL };

typedef struct {
    char symbol[15];
    long volume;
    float high;
    float low;
    int   nodays;
    char  dept[10];
    char* ptr2symbol;
}STOCK;

STOCK stock[] = {
    {"IOC", 11721000 , 121.70	,  117.75 ,  40 , "OIL" , NULL	} ,
    {"SUNPHARMA", 4222492 , 902.85 , 881.35 , 34 , "PHARMA" , NULL} ,
    {"ONGC", 17434362, 172.70	, 169.05 , 40 , "OIL" , NULL} ,
    {"HINDALCO", 12631663, 528.50	, 512.05 , 50, "PSU" , NULL} ,
    {"ASIANPAINT", 705765 , 3248.80	,  3176.05 , 70 , "CHEM"  , NULL	} ,
    {"COALINDIA", 1404000 , 169.35	,  160.00 , 30 , "PSU" , NULL	} ,
    {"MARUTI", 1404000 , 8989.80	,  8989.80 , 64, "AUTO" ,  NULL	} ,
    {"SHREECEM", 32000 , 25599.00	,  24677.85 , 10 , "CEMENT" ,  NULL	} ,
    {"INDUSINDBK", 4101000 , 971.40	,  940.40 , 130 , "BANK" , NULL	} ,
    {"BAJAJ-AUTO", 353000 , 3452.00	,  3559.00 , 110 , "AUTO" ,  NULL	} ,
    {NULL}
};

bool RECORD_DELETED = false;

int         findFieldName(FIELDS* f, char* qryName);
void        getFileName(char* file_name);
int         readDataBase(int MODE , char file_name[]);
int         readDataFromFile(MAIN* tmain, FILE* tfp);
void        dataBaseStruct(FIELDS f[]);
void        copyFieldToRecord(char* data, STOCK s, char ftype[], int foffset[]);
LISTS*      addToList(MAIN* tmain, void* data, int len);
void        freeList(MAIN* tmain);
void        appendData();
void        initMainLink();
int         getDataBaseFile(char* strSql, char file_name[]);
bool        addRecord(DB_MAIN* db_main, char *file_name);
void        editRecord(char *file_name);
void        *getRecordEdit(DB_MAIN* db_main, FIELDS fields[], char ftype[], int foffset[], int fwidth[]);
RET_RESULT  *getRecord(MAIN* tmain, int rec_no);
bool        deleteRecord(MAIN* tmain);
int         getRecordInput(DB_MAIN* db_main, FIELDS fields[], int fwidth[], char ftype[], int foffset[], char* file_name );
void        saveNewRecord(MAIN* tmain, char* file_name);
void        saveRecords(MAIN* tmain, char* file_name);

void        printHeaderQuery(SELECTFIELDS* s);
void        print_records(MAIN* tmain, char ftype[], int foffset[]);
void        printLine(int width);
void        printHeader(FIELDS f[]);
void        printRecord(void* data, char ftype[], int foffset[], int record_indx);


int getDataBaseFile(char *strSql , char file_name[]) {
    char* from = strFind(strSql, CHPTR "from");
    char* to = from + 1;
    int i = 0;

    //printf("\n%s", from);

    while (*(to + i) == 32) {
        i++;
    }

    while (*(to + i) != 32) {
        i++;
    }

    int len = (to + i) - from;
    strncpy(file_name, to, len);
    file_name[i] = NULL;

    skipSpace(file_name);

    trimSpace(file_name);
    return 1;
}

void getFileName(char* file_name) {
    system("cls");
    printf("\n\n\n\n");
    printf("\t\t\t\t");
    while (1) {
        fseek(stdin, 0, SEEK_END);
        printf("Enter File Name :: ");
        scanf("%[^\n]s", file_name);
        //strcpy(file_name, "x.txt");
        if (strlen(file_name) > 0) {
            break;
        }
    }
}


int readDataBase(int MODE , char file_name[]) {
    if (MODE == 0) {
       //strcpy(file_name, "stockfeb.txt");
       //strcpy(file_name, "car.txt");
        getFileName(file_name);
    }


    //fp = fopen(file_name, "r");
    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        system("cls");
        printf("\n Can't Open %s ", file_name);
        _getch();
        return NULL;
    }

    fread(&dbmain, sizeof(DB_MAIN), 1, fp);

    for (int i = 0; i < MAX_FIELD; i++)
    {
        Fields[i] = { NULL };
    }

    fread(Fields, sizeof(FIELDS) * MAX_FIELD, 1, fp);

    //char idata[256] = { NULL };

    //fread(idata, 63, 1, fp);

    system("cls");

    initMainLink();



    fseek(fp, 0, SEEK_SET);
    fseek(fp, sizeof(DB_MAIN) + (sizeof(FIELDS) * MAX_FIELD), SEEK_SET);
    readDataFromFile(lmain, fp);
    if (MODE == 0) {
        dataBaseStruct(Fields);
    }

    fseek(fp, 0, SEEK_SET);
    fwrite(&dbmain, sizeof(DB_MAIN), 1, fp);
    fwrite(Fields, sizeof(FIELDS) * MAX_FIELD, 1, fp);
    //printf("%d",MODE);

    fclose(fp);

    return 1;
    //printf("");
}

void dataBaseStruct(FIELDS f[]) {
    char strArray[50] = { NULL };
    int field_count = 0;
    int rec_size = 0;
    printf("\n\t\t==========================================================");
    printf("\n\t\tSl No\tField Name\t\tWidth\t\tDecimal");
    printf("\n\t\t==========================================================");

    for (int i = 0; i < MAX_FIELD; i++) {
        if (f[i].fname[0] == NULL) {
            break;
        }
        field_count++;
        rec_size = rec_size + f[i].fwidth;
        printf("\n\t\t%d\t%-10s\t\t%d\t\t%d", field_count, f[i].fname, f[i].fwidth, f[i].decimal);
    }

    printf("\n\n\t\t==========================================================");
    printf("\n\t\t No of Fields :: %d", field_count);
    printf("\n\t\t Record Size  :: %d", rec_size);
    printf("\n\t\t==========================================================");
    printf("\n\n\n \t\t Preass Any Key To Continue");
    _getch();
}

int readDataFromFile(MAIN* tmain, FILE* tfp)
{
    char* data;
    int data_read = 0;
    data = (char*)calloc(1, dbmain.record_size + DATA_PAD); // dbmain.record_size + 2

    if (tmain->start != NULL)
    {
        return 0;
    }

    int f_eof = 0;
    int OK = 1;
    int i = 0;
    int fpos = ftell(tfp);;
    while (OK) {
        data_read = fread(data, dbmain.record_size + DATA_PAD, 1, tfp);
        f_eof = feof(tfp);
        fpos = ftell(tfp);
        if (!f_eof) {
            addToList(tmain, data, dbmain.record_size + DATA_PAD);
        }
        else {
            break;
        }
        i++;
    }

    free(data);
}

void pasteDataToMem(char *data , char ftype,  char *strData , int len ) {
    int intVal = 0;
    float floatVal = 0;
    double doubleVal = 0;

    char ch = ' ';

    switch (ftype) {
        case 'T':
            memcpy(data, strData, len);
            break;

        case 'I':
            intVal = atoi(strData);
            memcpy(data, &intVal, 4);
            break;

        case 'F':
            floatVal = atof(strData);
            memcpy(data, &floatVal, 4);
            break;

        case 'D':
            //_atodbl(&doubleVal , strData);
            doubleVal = strtod(strData, NULL);
            memcpy(data, &doubleVal, 8);
            break;

        case 'C':
            ch = *strData;
            memcpy(data, &ch, 1);
            break;
    }
}

void initSrcData(int len) {
    for (int i = 0; i < len; i++) {
        srcData[i] = NULL;
    }
}

int splitStrForField(char* str) {
    char* temp = str;
    char* prevPos = NULL;
    char* data = NULL;

    int i = 0 , findx = 0 , n = 0;
    char DELIMETER = '|';
    char ch = 32;

    data = CHPTR calloc(1, dbmain.record_size + DATA_PAD);

    prevPos = str;
    //strFind(data, CHPTR "\r");
    

    while (1) {
        ch = *(str + i);
        if (ch == NULL || ch == '\r') {
            *(str + i) = NULL;
            srcData[n] = NULL;
            pasteDataToMem(data + dbmain.foffset[findx], dbmain.ftype[findx], srcData, dbmain.fwidth[findx]);
            n = 0;
            break;
        }else if (ch == '|') {
            //*(str + i) = NULL;
            srcData[n] = NULL;
            pasteDataToMem(data + dbmain.foffset[findx], dbmain.ftype[findx], srcData, dbmain.fwidth[findx]);
            prevPos = str + i + 1;
            findx++;
            n = 0;
            if (dbmain.ftype[findx] == 'T') {
                initSrcData(dbmain.fwidth[findx]);
            }
            else {
                initSrcData(20);
            }
        }
        else {
            srcData[n] = *(str + i);
            n++;
        }

        i++;
    }

    addToList(lmain, data, dbmain.record_size + DATA_PAD);

    free(data);
    return 0;
}

void get_FileName(char* file_name, char *msg) {
    system("cls");
    printf("\n\n\n\n");
    printf("\t\t\t\t");
    while (1) {
        fseek(stdin, 0, SEEK_END);
        printf("Enter %s File Name :: " , msg);
        scanf("%[^\n]s", file_name);
        //strcpy(file_name, "x.txt");
        if (strlen(file_name) > 0) {
            break;
        }
    }
}

void importTextData() {
    char csv_file_name[30] = { NULL };
    char fileName[30] = { NULL };
    char strLine[512] = { NULL };
    char* temp = NULL;
    char* data_read = NULL;
    int rec_count = 0;

    //strcpy(fileName, "stockfeb.txt");
    //strcpy(csv_file_name, "stockfeb.csv");

    //strcpy(fileName, "stockfeb.txt");
    //strcpy(csv_file_name, "stockfeb.csv");

    get_FileName(csv_file_name , CHPTR " Source ");
    get_FileName(fileName, CHPTR " Destination ");

    FILE *fp = fopen(fileName, "rb+");
    if (fp == NULL) {
        printf("\t\t\t Cant Open %s %s file", CHPTR "Destination ", fileName);
        system("cls");
        _getch();
        return (void)0;
    }


    FILE* fpCsv = fopen(csv_file_name, "rb");

    if (fpCsv == NULL) {
        printf("\t\t\t Cant Open %s %s file", CHPTR  " Source ", csv_file_name);
        system("cls");
        _getch();
        return (void )NULL;
    }


    //int ifd = open(fileName, O_BINARY | O_RDWR);
    //int ifdCsv = open(csv_file_name, O_BINARY || O_RDONLY );
   


    fread(&dbmain, sizeof(DB_MAIN), 1, fp);
    fread(Fields, sizeof(FIELDS), MAX_FIELD, fp);

    //read(ifd , &dbmain, sizeof(DB_MAIN));
    //read(ifd, Fields, sizeof(FIELDS) * MAX_FIELD);

    initMainLink();

    //fgets(strLine)

    fgets(strLine, 255, fpCsv);
    //read(ifdCsv, strLine, 255);
    int i = 0;
    while (1) {
        //int data_read = 0;
        char* data_read = NULL;
        data_read  = fgets(strLine, 255, fpCsv);

        //data_read = read(ifdCsv, strLine, 255);
        if (data_read == NULL) {
            break;
        }
        else {
            temp = strstr(strLine, "\r\n");
            if (temp != NULL) {
                *(temp + 0) = NULL;
                *(temp + 1) = NULL;
            }
            splitStrForField(strLine);
            rec_count++;
            printf("\n");
        }
        i++;
    }

    fseek(fp, 0, SEEK_SET);
    //lseek(ifd, 0, SEEK_SET);
    dbmain.record_count = rec_count;

    fwrite(&dbmain, sizeof(DB_MAIN), 1, fp);
    fwrite(Fields, sizeof(FIELDS), MAX_FIELD, fp);

    //write(ifd , &dbmain, sizeof(DB_MAIN));
    //write(ifd, &Fields, sizeof(FIELDS) * MAX_FIELD);

    //fseek(fp, sizeof(FIELDS) * MAX_FIELD , SEEK_CUR);

    fseek(fp, 0, SEEK_SET);
    fseek(fp, sizeof(DB_MAIN) + (sizeof(FIELDS) * MAX_FIELD), SEEK_CUR);

    //lseek(ifd, 0, SEEK_SET);
    //lseek(ifd, sizeof(DB_MAIN) + (sizeof(FIELDS) * MAX_FIELD), SEEK_CUR);

    //fwrite(&dbmain, sizeof(DB_MAIN), 1, fp);
    //fwrite(Fields, sizeof(FIELDS), MAX_FIELD, fp);
    //FILE* sfp = fopen("s.txt", "wb");

    LISTS* tlist = lmain->start;

    while (1) {
        if (tlist == NULL) {
            break;
       }

        //write(ifd, tlist->data, dbmain.record_size);
       int recWrite = fwrite(tlist->data, dbmain.record_size + DATA_PAD, 1, fp);
       //recWrite = fwrite(tlist->data, dbmain.record_size + DATA_PAD, 1, sfp);
       tlist = tlist->next;
    }

    freeList(lmain);

    //fclose(sfp);

    fclose(fp);
    fclose(fpCsv);

    //close(ifd);
    //close(ifdCsv);
    _getch();
}

void initMainLink() {
    if (lmain == NULL) {
        lmain = (MAIN*)calloc(1, sizeof(MAIN));
        lmain->start = NULL;
        lmain->end = NULL;
    }
    else {
        freeList(lmain);
        lmain->start = NULL;
        lmain->end = NULL;
    }

    if (seleFields) {
        free(seleFields);
    }
}

int findFieldName(FIELDS* f, char* fName) {
    bool FOUND = false;
    char fieldName[30];
    int i = 0;

    while (1) {
        if ((f + i)->fname[0] == NULL) {
            break;
        }
        else {
            strcpy_s(fieldName, (f + i)->fname);
            int cmpVal = strcmp(strToUpper(fieldName), strToUpper(fName));
            if (cmpVal == 0) {
                FOUND = true;
                break;
            }
        }
        i++;
    }

    if (FOUND) {
        return i;
    }
    else {
        return -1;
    }
}

bool addRecord(DB_MAIN* db_main, char *file_name) {
    char choice = ' ';
    getRecordInput(db_main, Fields, db_main->fwidth, db_main->ftype, db_main->foffset, file_name);

    return true;
}

int  getRecordInput(DB_MAIN* db_main, FIELDS fields[], int fwidth[], char ftype[], int foffset[] , char *file_name) {
    char* data = NULL, * fdata = NULL;
    int i = 0;
    int fcount = 0;
    int intVal = 0;
    float floatVal = 0;
    double doubleVal = 0;
    char ch = ' ', choice = ' ';
    int record_added = 0;


    while (1) {
        data = (char*)calloc(1, db_main->record_size);
        for (int f = 0; f < db_main->field_count; f++) {
            switch (ftype[f])
            {
            case 'T':
                fseek(stdin, 0, SEEK_END);
                fdata = (char*)calloc(1, fields[f].fwidth);
                printf("Enter %s :: ", fields[f].fname);
                scanf("%[^\n]s", fdata);
                memcpy(data + foffset[f], fdata, db_main->fwidth[f]);
                free(fdata);
                break;

            case 'I':
                fseek(stdin, 0, SEEK_END);
                printf("Enter %s :: ", fields[f].fname);
                scanf("%d", &intVal);
                memcpy(data + foffset[f], &intVal, sizeof(int));
                break;

            case 'F':
                fseek(stdin, 0, SEEK_END);
                printf("Enter %s :: ", fields[f].fname);
                scanf("%f", &floatVal);
                memcpy(data + foffset[f], &floatVal, sizeof(float));
                break;

            case 'D':
                fseek(stdin, 0, SEEK_END);
                printf("Enter %s :: ", fields[f].fname);
                scanf("%lf", &doubleVal);
                memcpy(data + foffset[f], &doubleVal, sizeof(double));
                break;
            case 'C':
                fseek(stdin, 0, SEEK_END);
                printf("Enter %s :: ", fields[f].fname);
                scanf("%c", &ch);
                memcpy(data + foffset[f], &ch, sizeof(char));
                break;



            default:
                break;
            }

            //fcount++;
            if (fields[f].fname[0] == NULL) {
                break;
            }
        }

        while (1) {
            printf("\n\nAdd One More Record [Y/N]  :: ");
            //choice = 32;
            fseek(stdin, 0, SEEK_END);
            scanf("%c", &choice);
            if (toupper(choice) == 'Y' || toupper(choice) == 'N') {
                break;
            }
        }

        addToList(lmain, data, db_main->record_size +DATA_PAD);
        saveNewRecord(lmain, file_name);

        record_added++;

        if (toupper(choice) == 'N') {
            break;
        }
    }

    return record_added;
}


void editRecord(char *file_name) {
    int rec_no = 0;
    printf("\n\n\n\n");
    printf("\t\t\t\t");
    while (1) {
        system("cls");
        fseek(stdin, 0, SEEK_END);
        printf("Enter Record No :: ");
        scanf("%d", &rec_no);
        if (rec_no > 0) {
            break;
        }
    }

    RET_RESULT* retResult = getRecord(lmain, rec_no);

    if (retResult->ret_code != 4) {
        printf("\n\n Record Not Found ");
    }
    else {
        void* data = NULL;
        data = getRecordEdit(&dbmain, Fields, dbmain.ftype, dbmain.foffset, dbmain.fwidth);
        memcpy(((LISTS*)retResult->ret_adrs)->data, data, dbmain.record_size);
        fopen(file_name, "rb+");
        fseek(fp, sizeof(DB_MAIN) + sizeof(FIELDS) * MAX_FIELD, SEEK_SET);
        if (rec_no > 1) {
            fseek(fp, (rec_no - 1) * dbmain.record_size, SEEK_SET);
        }
        fwrite(data, dbmain.record_size + DATA_PAD, 1, fp);
        fclose(fp);
        free(data);
    }

}

void* getRecordEdit(DB_MAIN* db_main, FIELDS fields[], char ftype[], int foffset[], int fwidth[]) {
    char* data = NULL, * strVal = NULL;
    int i = 0;
    int fcount = 0;
    int intVal = 0;
    char ch = ' ', choice = ' ';
    int record_added = 0;

    //char ftypes[] = { db_main->ftype };

    data = (char*)calloc(1, db_main->record_size);
    for (int f = 0; f < db_main->field_count; f++) {
        switch (ftype[f])
        {
        case 'I':
            fseek(stdin, 0, SEEK_END);
            printf("Enter %s :: ", fields[f].fname);
            scanf("%d", &intVal);
            memcpy(data + foffset[f], &intVal, sizeof(int));
            break;

        case 'C':
            fseek(stdin, 0, SEEK_END);
            printf("Enter %s :: ", fields[f].fname);
            scanf("%c", &ch);
            memcpy(data + foffset[f], &ch, sizeof(char));
            break;

        case 'T':
            fseek(stdin, 0, SEEK_END);
            strVal = (char*)calloc(1, fields[f].fwidth);
            printf("Enter %s :: ", fields[f].fname);
            scanf("%[^\n]s", strVal);
            memcpy(data + foffset[f], strVal, db_main->fwidth[f]);
            free(strVal);
            break;

        default:
            break;
        }

        if (fields[f].fname[0] == NULL) {
            break;
        }
    }

    while (1) {
        printf("\n\nSave Record [Y/N]  :: ");
        fseek(stdin, 0, SEEK_END);
        scanf("%c", &choice);
        if (toupper(choice) == 'Y' || toupper(choice) == 'N') {
            break;
        }
    }

    return data;
    //saveNewRecord(lmain);
}

RET_RESULT* getRecord(MAIN* tmain, int rec_no) {
    if (tmain == NULL) {
        ret_result->ret_adrs = NULL;
        ret_result->ret_code = 1;
        return ret_result;
    }

    if (tmain->start == NULL) {
        ret_result->ret_adrs = NULL;
        ret_result->ret_code = 2;
        return ret_result;
    }

    LISTS* tlist = tmain->start;

    int r = 0;
    bool FOUND = false;
    while (1) {
        if (tlist == NULL || r > rec_no - 1) {
            ret_result->ret_adrs = NULL;
            ret_result->ret_code = 3;
            FOUND = false;
            break;
        }

        if (r == rec_no - 1) {
            FOUND = true;
            break;
        }

        tlist = tlist->next;
        r++;
    }

    if (FOUND) {
        ret_result->ret_adrs = tlist;
        ret_result->ret_code = 4;
    }

    return ret_result;
}

void saveNewRecord(MAIN* tmain, char *file_name) {
    LISTS* tlist, * temp;

    fp = fopen(file_name, "rb+");

    //fseek(fp, sizeof(DB_MAIN) + (sizeof(FIELDS) * 32) + (dbmain.record_size *  dbmain.record_count ) , SEEK_SET);
    fseek(fp, 0, SEEK_END);

    if (tmain->end != NULL) {
        tlist = tmain->end;
        fwrite(tlist->data, dbmain.record_size + DATA_PAD, 1, fp);
        dbmain.record_count++;
        fseek(fp, 0, SEEK_SET);
        fwrite(&dbmain, sizeof(DB_MAIN), 1, fp);
    }

    fclose(fp);
}

void saveRecords(MAIN* tmain, char *file_name) {
    if (!RECORD_DELETED) {
        return;
    }

    LISTS* tlist, * temp;
    //void* temp;

    fp = fopen(file_name, "wb+");
    fwrite(&dbmain, sizeof(DB_MAIN), 1, fp);
    fwrite(Fields, sizeof(FIELDS) * MAX_FIELD, 1, fp);

    tlist = tmain->start;
    while (1) {
        if (tlist == NULL) {
            break;
        }

        fwrite(tlist->data, dbmain.record_size + DATA_PAD, 1, fp);

        temp = (LISTS*)tlist->next;
        tlist = temp;
        dbmain.record_count++;
    }

    fseek(fp, 0, SEEK_SET);
    fwrite(&dbmain, sizeof(DB_MAIN), 1, fp);

    RECORD_DELETED = false;
    fclose(fp);
}
bool deleteRecord(MAIN* tmain) {
    LISTS* pres, * temp, * prev = NULL;
    int record_no = 0;

    if (dbmain.record_count == 0) {
        return false;
    }

    while (1) {
        system("cls");
        printf("\n\nEnter Record No To Delete  :: ");
        fseek(stdin, 0, SEEK_END);
        scanf("%d", &record_no);
        if (record_no > 0) {
            break;
        }
    }

    pres = tmain->start;

    int i = 1;
    while (1) {
        if (i < record_no) {
            prev = pres;
            pres = pres->next;
        }
        else {
            break;
        }
        i++;
    }

    if (record_no == 1) {
        temp = pres->next;
        tmain->start = temp;
    }
    else if (dbmain.record_count == record_no) {
        tmain->end = prev;
        prev->next = NULL;
    }
    else if (record_no > 1 && (dbmain.record_count > record_no)) {
        temp = pres->next;
        prev->next = temp;
    }

    free(pres->data);
    free(pres);
    dbmain.record_count--;

    if (tmain->start == NULL) {
        tmain->end = NULL;
    }

    RECORD_DELETED = true;
    return true;
}

LISTS* addToList(MAIN* tmain, void* data, int len) {

    LISTS* tlist = (LISTS*)calloc(1, sizeof(LISTS));


    if (tmain->start == NULL) {
        tmain->start = tlist;
        tmain->end = tlist;
        tlist->prev = NULL;
        tlist->next = NULL;
    }
    else {
        LISTS* tend  = NULL;
        tend = tmain->end;
        tend->next = tlist;

        tlist->prev = tend;
        tlist->next = NULL;
        tlist->next = NULL;
        //tmain->end->next = tlist;
        //tmain->end = tlist;
        //tlist->next = NULL;
        tmain->end = tlist;
    }

    tlist->data = calloc(1, len);

    void *tdata = memcpy(tlist->data, data, len);

    return tlist;
}

void  freeList(MAIN* tmain) {
    LISTS* tlist = tmain->start;
    LISTS* temp = NULL;
    char* data;

    while (1) {
        if (tlist == NULL) {
            tmain->start = NULL;
            tmain->end = NULL;
            break;
        }

        temp = tlist->next;
        data = (char*)tlist->data;

        //printf("\n %-15s %15u ", data, *(int*)(data + 15));
        //printf(" %10.2f  %10.2f", *(float*)(data + 19), *(float*)(data + 23));
        //printf(" %10d  %-10s", *(int*)(data + 27), (data + 31));
        
        free(tlist->data);
        free(tlist);
        tlist = temp;
    }
}

void appendData() {
    int i = 0;
    int   fOffset[10] = { 0, 15, 19, 23, 27 , 31 , 41 , 0 };
    char  ftype[] = "TIFFIT";
    int lenRec = sizeof(STOCK);
    void* data = calloc(1, sizeof(STOCK));

    while (1) {
        STOCK s = stock[i];
        if (s.symbol[0] == NULL) {
            break;
        }

        copyFieldToRecord((char*)data, s, ftype, fOffset);
        addToList(lmain, data, sizeof(STOCK));

        i++;
        if (i >= 10) {
            break;
        }
    }

    free(data);
}

void copyFieldToRecord(char* data, STOCK s, char ftype[], int foffset[]) {
    int intVal = 0;
    float floatVal = 0;
    char ch = ' ';

    for (int i = 0; i < 6; i++) {
        switch (i) {
        case 0:
            memcpy(data + foffset[i], s.symbol, 15);
            break;

        case 1:
            intVal = s.volume;
            memcpy(data + foffset[i], &intVal, sizeof(int));
            break;

        case 2:
            floatVal = s.high;
            memcpy(data + foffset[i], &floatVal, sizeof(float));
            break;

        case 3:
            floatVal = s.low;
            memcpy(data + foffset[i], &floatVal, sizeof(float));
            break;

        case 4:
            intVal = s.nodays;
            memcpy(data + foffset[i], &intVal, sizeof(int));
            break;

        case 5:
            memcpy(data + foffset[i], s.dept, 10);
            break;
        default:
            break;
        }
    }
}

void printRecord(void* data, char ftype[], int foffset[], int record_indx ) {
    int field_count = 0, lineCount = 0;
    char field_format[50] = { NULL }, strData[50] = { NULL };
    printf("\t %-4d", record_indx);
    for (int i = 0; i < dbmain.field_count; i++) {
        switch (ftype[i])
        {
        case 'T':
            if (strlen(((char*)data + foffset[i])) >= 30) {
                strncpy(strData, ((char*)data + foffset[i]), 30);
                strData[30] = NULL;
            }
            else {
                strcpy(strData, ((char*)data + foffset[i]));
            }
            sprintf(field_format, " %%-%ds", dbmain.fwidth[i] >= 30 ? 30 : dbmain.fwidth[i]);
            printf(field_format, strData); // ((char*)data + foffset[i]));
            break;

        case 'I':
            printf(" %15d", *(int*)((char*)data + foffset[i]));
            break;

        case 'F':
            printf(" %10.2f", FLOATPTRVAL(CHPTR data + foffset[i])); // seleFields[i].Fields.foffset));
            break;

        case 'D':
            printf(" %15.2f", *(double*)(CHPTR data + foffset[i])); // seleFields[i].Fields.foffset));
            break;

        case 'C':
            printf(" %5c", *(char*)((char*)data + foffset[i]));
            break;

        default:
            break;
        }
    }

    printf("\n");
}

void printHeader(FIELDS f[]) {
    char strArray[50] = { NULL };

    //printf("\n\t================================================================\n");

    printLine(180);
    printf("\tSl No");
    for (int i = 0; i < MAX_FIELD; i++) {
        if (f[i].fname[0] == NULL) {
            break;
        }
        if (f[i].ftype == 'T') {
            sprintf(strArray, " %%-%ds", f[i].fwidth >= 30 ? 30 : f[i].fwidth);
        }
        else if (f[i].ftype == 'I') {
            sprintf(strArray, " %%12s");
        }
        else if (f[i].ftype == 'F') {
            sprintf(strArray, " %%12s");
        }
        else if (f[i].ftype == 'D') {
            sprintf(strArray, " %%12s");
        }
        else if (f[i].ftype == 'C') {
            sprintf(strArray, " %%5s");
        }

        printf(strArray, f[i].fname);
    }

    //printf("\n\t================================================================\n");
    printLine(180);
}

void printLine(int width) {
    printf("\n\t");
    for (int i = 0; i <= width; i++) {
        printf("%c", '=');
    }
    printf("\n");
}

void print_records(MAIN* tmain, char ftype[], int foffset[]) {
    LISTS* tlist, * temp;
    int record_no = 0, lineCount = 0;

    //void* temp;

    system("cls");

    tlist = tmain->start;

    printHeader(Fields);
    while (1) {
        if (tlist == NULL) {
            break;
        }

        record_no++;
        printRecord(tlist->data, ftype, foffset, record_no);
        tlist = tlist->next;
        lineCount++;
        if (lineCount > 21) {
            lineCount = 0;
            printf("\n Press Any Key Continue");
            _getch();
            system("cls");
            printHeader(Fields);
        }
    }
    printf("\n Press Any Key go to Main Menu");
    _getch();
}

void printHeaderQuery(SELECTFIELDS* s) {
    char strArray[50] = { NULL };
    int width = 0;
    FIELDS f;

    for (int i = 0; i < MAX_FIELD; i++) {
        f = (s + i)->Fields;
        if (f.fname[0] == NULL) {
            break;
        }
        width = width + f.fwidth;
    }

    printLine(180);
    printf("\tSl No");
    for (int i = 0; i < MAX_FIELD; i++) {
        f = (s + i)->Fields;
        if (f.fname[0] == NULL) {
            break;
        }
        if (f.ftype == 'T') {
            sprintf(strArray, " %%-%ds", f.fwidth < 30 ? f.fwidth : 30);
        }
        else if (f.ftype == 'I') {
            sprintf(strArray, "%%15s");
        }
        else if (f.ftype == 'F') {
            sprintf(strArray, "%%15s");
        }
        else if (f.ftype == 'D') {
            sprintf(strArray, "%%15s");
        }
        else if (f.ftype == 'C') {
            sprintf(strArray, "%%5s");
        }

        printf(strArray, f.fname);
    }

    printLine(180);
    //printf("\n\t================================================================\n");
}
