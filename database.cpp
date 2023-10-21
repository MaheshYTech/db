#pragma warning(disable : 4996)

#include <cstdlib>
#include <errno.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <io.h>
#include <conio.h>
//#include "data.h"
//#include <iostream>

#include "std_define.h"
#include "db.h"
#include "query.h"
#include "mylib.h"


//FIELDS fields[32] = { NULL };

char fileName[32] = { NULL };


void            scanfSolution();
//LISTS            *addToList(MAIN* tmain, void* data, int len);
int             freeMem(MAIN* tmain);
void            createDataBase();
void            getFields(FIELDS* field);
void            dataBaseStruct(FIELDS f[]);
void            getFieldName(char* fname);
void            getFieldType(char* ftype);
int             getFieldWidth();
void            getFileName(char* file_name);
//void            print_records(MAIN* tmain, char ftype[], int foffset[]);
//void            printRecord(void* data, char ftype[], int foffset[], int record_no);
//void            printHeader(FIELDS f[]);
//int             readDataFromFile(MAIN* tmain, FILE* tfp);
void            displayMenu();
void            displayUpdateMenu();
bool            deleteRecord(MAIN* tmain);
int             startSql(int MODE, char* strSql);
void            printHeaderQuery(SELECTFIELDS* s);
void            printLine(int width);

//void            freeList(MAIN* tmain);

LISTS* list;

bool isIntString(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }

    return true;
}

void readTempFile() {
    //FILE* tfp = fopen("s.txt", "rb");
    //long tfd = _open("stocks.txt", O_CREAT || O_RDWR);

    char strBuff[255] = { NULL };
    int data_read = 0;

    int ifdCSV = open("stocks.csv", O_BINARY | O_RDONLY);
    
    int readByte = read(ifdCSV, strBuff, 255);

    printf("%d", ifdCSV);

    

    _getch();

    //data_read = fread(strBuff, 63, 1, tfp);
    //data_read = _read(tfd, strBuff, sizeof(DB_MAIN));

    //printf("\n %s  %d", strBuff, data_read);

    //_close(tfd);

    //fclose(tfp);
}

//char sql[255] = { "select symbol, volume,  nodays, dept from db.txt where nodays > 50 or dept = 'AUTO'" };
//char sql[255] = { "select name, age,  dept from db.txt where age >= 64 " };
char sql[255] = { "select name, age,  dept from db.txt where age > 50 and age < 55 " };

int main()
{
    int val = 3625251;
    //printf("%d %d", val, sizeof(int));

    //readTempFile();

    ret_result = (RET_RESULT*)calloc(1, sizeof(RET_RESULT));


    initMainLink();

    fileName[0] = { NULL };
    //startSql(1, fileName);

    displayMenu();

    freeMem(lmain);
    free(lmain);

    free(ret_result);

    printf("\n\t\t\t\t\t Press Any Key to Exit  ");
    _getch();
}


void displayMenu() {
    char ch = ' ';
    bool wloop = true;
    int choice = 0;

    while (wloop) {
        system("cls");
        printf("\n\n\n");

        printf("\n \t\t\t\t ------------------------------------------");
        printf("\n \t\t\t\t\t    ****** Main Menu ******               ");
        printf("\n \t\t\t\t ------------------------------------------");
        printf("\n \t\t\t\t\t\t 1. Create Database   ");
        printf("\n \t\t\t\t\t\t 2. Open Database     ");
        printf("\n \t\t\t\t\t\t 3. Update Database   ");
        printf("\n \t\t\t\t\t\t 4. Import From Files ");
        printf("\n \t\t\t\t\t\t 5. Save Data Base    ");
        printf("\n \t\t\t\t\t\t 6. Display Data      ");
        printf("\n \t\t\t\t\t\t 7. SQL               ");
        printf("\n \t\t\t\t\t\t 8. Close Data Base   ");
        printf("\n \t\t\t\t\t\t 0. Exit              ");

        printf("\n");
        printf("\n \t\t\t\t ------------------------------------------");
        printf("\n \t\t\t\t\t Enter Your Choice  ==>   ");
        fseek(stdin, 0, SEEK_END);
        //getline(cin, choices);
        scanf("%c", &ch);

        choice = ch - 48;

        switch (choice) {
        case 1:
            createDataBase();
            //getFields(fields);
            break;

        case 2:
            readDataBase(0, fileName);
            break;

        case 3:
            displayUpdateMenu();
            break;

        case 4:
            importTextData();
            break;

        case 6:
            print_records(lmain, dbmain.ftype, dbmain.foffset);
            break;

        case 7:
            fileName[0] = { NULL };
            startSql(1, fileName);
            break;

        case 0:
            wloop = false;
            break;
        }
    }
}

void displayUpdateMenu() {
    char ch = ' ';
    bool updateLoop = true;
    int choice = 0;

    while (updateLoop) {
        system("cls");
        printf("\n\n\n");

        printf("\n \t\t\t\t ------------------------------------------");
        printf("\n \t\t\t\t\t         Update Menu                     ");
        printf("\n \t\t\t\t ------------------------------------------");

        printf("\n \t\t\t\t\t\t 1. Add Record ");
        printf("\n \t\t\t\t\t\t 2. Edit Record");
        printf("\n \t\t\t\t\t\t 3. Delete Record");
        printf("\n \t\t\t\t\t\t 4. Display Records ");
        printf("\n \t\t\t\t\t\t 0. Back ");

        printf("\n");
        printf("\n \t\t\t\t ------------------------------------------");
        printf("\n \t\t\t\t Enter Your Choice  ==> ");
        //printf("\n \t\t ------------------------------------------");

        //printf("\t\t\t\t\t Enter Your Choice  ==>   ");
        fseek(stdin, 0, SEEK_END);
        //getline(cin, choices);
        scanf("%c", &ch);

        choice = ch - 48;

        switch (choice) {
        case 1:
            system("cls");
            addRecord(&dbmain, fileName);
            break;

        case 2:
            editRecord(fileName);
            system("cls");
            break;

        case 3:
            system("cls");
            deleteRecord(lmain);
            saveRecords(lmain, fileName);
            break;

        case 4:
            system("cls");
            print_records(lmain, dbmain.ftype, dbmain.foffset);
            break;

        case 5:
            system("cls");
            //saveRecords(lmain);
            break;

        case 0:
            system("cls");
            updateLoop = false;
            break;
        }
    }
}




//void freeList(MAIN *tmain) {
//    LISTS* tlist = tmain->start;
//    LISTS* temp = NULL;
//    while (1) {
//        if (tlist == NULL) {
//            break;
//        }
//
//        temp = tlist->next;
//        free(tlist->data);
//        free(tlist);
//        tlist = temp;
//    }
//
//    free(tmain);
//}

//void verifyList() {
//    LISTS* listArr[10] = { NULL };
//    EMP emp = { 1001,  "Mr.X" , 40, "IT",  'Y' };
//    listArr[0] = addToList(lmain, (void*)&emp, sizeof(EMP));
//
//    emp = { 1002,  "Bill Gates" , 64,  "CEO" , 'Y' };
//    ////strcpy(emp.name, "Bill Gates");
//    //emp.age = 64;
//    //emp.perm_emp = 'Y';
//    listArr[1] = addToList(lmain, (void*)&emp, sizeof(EMP));
//    displayData(lmain);
//
//
//
//    for (int i = 0; i < 2; i++) {
//        if (listArr[i] == NULL) {
//            break;
//        }
//        free(listArr[i]->data);
//        free(listArr[i]);
//    }
//}

void createDataBase() {
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
    getFileName(fileName);
    getFields(Fields);
}

void getFields(FIELDS* field) {
    char choice = 'Y', ch = ' ';
    int field_count = 0;
    int record_size = 0;
    char fwidth[5] = {};

    for (int i = 0; i < MAX_FIELD; i++) {
        FIELDS f = {};

        getFieldName(f.fname);
        getFieldType(&f.ftype);

        if (f.ftype == 'I' || f.ftype == 'F') {
            f.fwidth = sizeof(int);
            printf("\n Field Width :: %d", f.fwidth);
        }
        else if (f.ftype == 'D') {
            f.fwidth = sizeof(double);
            printf("\n Field Width :: %d", f.fwidth);
        }
        else {
            f.fwidth = getFieldWidth();
        }



        if (f.ftype == 'F' || f.ftype == 'D') {
            fseek(stdin, 0, SEEK_END);
            printf("\n Field Decimal Place :: ");
            scanf("%d%*c", &f.decimal);
        }

        while (1) {
            printf("\n\nAdd One More Field [Y/N]  :: ");
            fseek(stdin, 0, SEEK_END);
            scanf("%c", &choice);
            if (toupper(choice) == 'Y' || toupper(choice) == 'N' || choice == '\n') {
                break;
            }
        }

        f.foffset = record_size;
        field_count++;
        dbmain.foffset[i] = record_size;
        record_size = record_size + f.fwidth;

        Fields[i] = f;
        dbmain.fwidth[i] = f.fwidth;
        dbmain.ftype[i] = f.ftype;
        dbmain.record_size = record_size;
        dbmain.field_count = field_count;


        if (choice == 'N' || choice == 'n') {
            break;
        }

        printf("\n");
    }

    strcpy(dbmain.dbname, "DIANA");
    strcpy(dbmain.dbversion, "4.1.4");


    fd = open(fileName, O_BINARY | O_CREAT | O_RDWR );

    write(fd, &dbmain, sizeof(DB_MAIN));
    write(fd, &Fields, sizeof(FIELDS)*MAX_FIELD);

    //fwrite(&dbmain, sizeof(DB_MAIN), 1, fp);
    //fwrite(Fields, sizeof(FIELDS), MAX_FIELD, fp);

    //fclose(fp);

    close(fd);

    printf("");
}


void getFieldName(char* fname) {
    while (1) {
        fseek(stdin, 0, SEEK_END);
        printf("\n Field Name :: ");
        scanf("%[^\n]s", fname);
        if (fname[0] != '\0') {
            break;
        }
    }
}

void getFieldType(char* ftype) {
    while (1) {
        fseek(stdin, 0, SEEK_END);
        //*ftype = NULL;
        printf("\n Field Type [I (Int)  , T (Text) , C (Char),  F (Float) ] :: ");
        scanf("%c", ftype);
        if (toupper(*ftype) == 'I' || toupper(*ftype) == 'T' || toupper(*ftype) == 'C' || toupper(*ftype) == 'F' || toupper(*ftype) == 'D') {
            *ftype = toupper(*ftype);
            break;
        }
    }
}

int getFieldWidth() {
    char fwidth[5] = {};

    while (1) {
        fseek(stdin, 0, SEEK_END);
        fwidth[0] = NULL;
        printf("\n Field Width :: ");
        //scanf("%d%*c", &f.fwidth);
        scanf("%[^\n]s", fwidth);

        if (isIntString(fwidth)) {
            if (atoi(fwidth) > 0) {
                //return atoi(fwidth);
                break;
            }
        }
        //fseek(stdin, 0, SEEK_END);
    }

    return atoi(fwidth);
}


int deleteSqlRecords(MAIN *tmain) {
    int WHERE_RESULT = 0;
    int rec_count = 0;
    int deleted_rec = 0;
    char  *data  = NULL;
    LISTS *temp  = NULL;
    LISTS *tlist = NULL;
    LISTS *prev  = NULL;
    LISTS *next  = NULL;

    tlist = tmain->start;

    while (1) {
        if (tlist ==  NULL) {
            break;
        }

        data = (char *)tlist->data;

        if (!WHERE_COND_MISSING) {
            WHERE_RESULT = verifyWhereCondition(data);
        }
        else {
            WHERE_RESULT = 1;
        }

        if (WHERE_RESULT) {   
            prev = tlist->prev;
            next = tlist->next;

            free((char *)tlist->data);
            free(tlist);

            if (prev != NULL) {
                prev->next = next;
            }
            else {
                //prev->next = NULL;
            }

            if (next != NULL) {
                next->prev = prev;
            }
            else {
                //next->prev = NULL;
            }

            tlist = next;
            if (rec_count == 0) {
                tmain->start = tlist;
            }


            deleted_rec++;
        }else {
            tlist = tlist->next;
            rec_count++;
        }
    }

    FILE* fp = fopen(fileName, "w+b");

    fread(&dbmain, sizeof(DB_MAIN), 1, fp);
    fread(Fields, sizeof(FIELDS) * MAX_FIELD, 1, fp);

    dbmain.record_count = rec_count;

    fwrite(&dbmain, sizeof(DB_MAIN), 1, fp);
    fwrite(Fields, sizeof(FIELDS) * MAX_FIELD, 1, fp);

    fseek(fp, 0, SEEK_SET);
    fseek(fp, sizeof(DB_MAIN) + (sizeof(FIELDS) * MAX_FIELD), SEEK_SET);

    tlist = tmain->start;

    int rcount = 0;
    while (1) {
        if (tlist == NULL) {
            break;
        }
        data = CHPTR tlist->data;
        //printf("\n %s %d", CHPTR data, INTPTRVAL(data + 54));
        fwrite(data, dbmain.record_size, 1, fp);
        tlist = tlist->next;
        rcount++;
    }

    fclose(fp);
    //_getch();
    return deleted_rec;
}

int getSqlType(char* sqlStr) {
    int i = 0;
    while (1) {
        if (*(sqlStr + i) != 32) {
            break;
        }
        i++;
    }

    char* seleStr = NULL;
    seleStr = strFind(sqlStr + i, CHPTR "select");
    if (seleStr != NULL) {
        return SELECT;
    }

    seleStr = strFind(sqlStr + i, CHPTR "delete");
    if (seleStr != NULL) {
        return DELETE;
    }

    seleStr = strFind(sqlStr + i, CHPTR "update");
    if (seleStr != NULL) {
        return UPDATE;
    }

    return -1;
}

int startSql(int MODE, char* strSql) {
    //openDataBase(strSql, fileName);
    int result = 0, i = 0, len = 0, wCount = 0 , OK = 1;
    char *prevPoint = NULL;
    
    sql[0] = NULL;

    while (OK) {
            char* strArr[] = { NULL };
            sql[0] = NULL;

            getSqlStatement(sql);
            result = 0;
            i = 0;
            len = 0;
            wCount = 0;

            if (strlen(sql) == 0) {
                printf("\n\n\n\n");
                printf("\t\t");
                break;
            }

            prevPoint = NULL;
            //strcpy(sql, "select Car, MPG, Cyly, HP, Model ,  Origin from car.txt where Cyly = 8");
            //strcpy(sql, "select * from car.txt where Cyly = 8");

            result = getDataBaseFile(sql, fileName);
            result = readDataBase(MODE, fileName);
            if (result == NULL) {
                printf("\n Error Opening File %s", fileName);
                _getch();
                break;
                //return 0;
            }

            i = 0;
            while (1) {
                if (*(sql + i) != 32) {
                    break;
                }
                i++;
            }


            int sqlType = getSqlType(sql);

            if (sqlType == SELECT) {
                result = getSelectField(sql);
                if (result == NULL) {
                    printf("\n Error in Select Fields");
                    _getch();
                    break;
                    //return 0;
                }
                result = processWhere(sql);
                if (result == NULL) {
                    printf("\n Error in Where Fields");
                    _getch();
                    break;
                    //return 1;
                }
                printHeaderQuery(seleFields);
                printQueryResult(lmain, &dbmain);
                if (seleFields) {
                    free(seleFields);
                    seleFields = NULL;
                }
                _getch();
                //return 0;
            }
            else if (sqlType == DELETE) {
                result = processWhere(sql + i);
                int deleted_rec = deleteSqlRecords(lmain);
                printf("\n\n\n\n");
                printf("\t\t");
                printf(" %d record has been deleted" , deleted_rec);
                printf("\n\t\t  Press Any Key To Continue");
                _getch();
                deleted_rec = deleted_rec + 0;
                //return 0;
            }
            else if (sqlType == UPDATE) {

            }
    }
}







int  getRecordInput(DB_MAIN* db_main, FIELDS fields[], int fwidth[], char ftype[], int foffset[]) {
    char* data = NULL, * fdata = NULL;
    int i = 0;
    int fcount = 0;
    int intVal = 0;
    char ch = ' ', choice = ' ';
    int record_added = 0;


    while (1) {
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
                fdata = (char*)calloc(1, fields[f].fwidth);
                printf("Enter %s :: ", fields[f].fname);
                scanf("%[^\n]s", fdata);
                memcpy(data + foffset[f], fdata, db_main->fwidth[f]);
                free(fdata);
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

        addToList(lmain, data, db_main->record_size);
        saveNewRecord(lmain, fileName);

        record_added++;

        if (toupper(choice) == 'N') {
            break;
        }
    }

    return record_added;
}


void print_record(MAIN* tmain, char ftype[], int foffset[]) {
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


void printRecords(void* data, char ftype[], int foffset[], int record_indx) {
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

//void printArray(char **strArray , int fwidth[] ,  char ftype[] ,int foffset[]) {
//    // { 'I', 'T', 'I', 'T', 'C', 'P' };
//    int field_count = 0;
//    void* data = calloc(1, 100);
//    int intVal = 0;
//    char ch = ' ';
//    char str[35] = { NULL };
//
//    for (int i = 0; i <= 4; i++) {
//        switch (ftype[i])
//        {
//            case 'I':
//                printf("\nEnter %s :: ", (char*)(strArray[i]));
//                scanf("%d%*c", &intVal);
//                memcpy((char*)data + foffset[i],  &intVal, fwidth[i]);
//                break;
//
//            case 'T':
//                printf("\nEnter %s :: ", (char*)(strArray[i]));
//                scanf("%[^\n]s", str);
//                memcpy((char*)data + foffset[i], str, fwidth[i]);
//                break;
//
//            case 'C':
//                printf("\nEnter %s :: ", (char*)(strArray[i]));
//                scanf("%*c%c%*c", &ch);
//                memcpy((char*)data + foffset[i], &ch, fwidth[i]);
//                field_count++;
//                break;
//
//            default:
//                break;
//        }
//        //printf("\n%s", (char *)(strArray[i]));
//    }
//
//    printf("\n");
//
//    fp = fopen("emp.txt", "w+");
//
//    fwrite(data, 54, 1, fp);
//
//    for (int i = 0; i <= 54; i++) {
//        *((char *)data + i) = NULL;
//    }
//    
//    fseek(fp, 0, SEEK_SET);
//
//    fread(data, 54, 1, fp);
//
//    printData(strArray, data, ftype, foffset);
//
//    fclose(fp);
//    free(data);
//}



//void printData(char **strArray, void *data , char ftype[],  int foffset[] ) {
//    int field_count = 0;
//
//    for (int i = 0; i <= 4; i++) {
//        switch (ftype[i])
//        {
//            case 'I':
//                printf("\n%s  :: %d", (char*)(strArray[i]), *(int*)((char*)data + foffset[i]));
//                break;
//
//            case 'T':
//                printf("\n%s :: %s", (char*)(strArray[i]), ((char*)data + foffset[i]));
//                break;
//
//            case 'C':
//                printf("\n%s :: %c", (char*)(strArray[i]), *(char*)((char*)data + foffset[i]));
//                field_count++;
//                break;
//
//            default:
//                break;
//        }
//    }
//}

void scanfSolution() {
    int x = 0, y = 0;
    char str[100];
    char ch1 = 'a', ch2 = 'b';
    printf("\nEnter x Value :: ");
    scanf("%d%*c", &x);
    printf("\nEnter y Value :: ");
    scanf("%d%*c", &y);
    printf("\nEnter  Character :: ");
    scanf(" %c%*c", &ch1);
    ////printf("\nEnter  Character :: ");
    //scanf("%*c", &ch2);
    //fflush(stdin);
    //fseek(stdin, 0, SEEK_END);
    printf("\nEnter  String :: ");
    scanf("%[^\n]s", &str);
    //fgets(str, 100, stdin);
    printf("x = %d, y = %d , ch1 = %c , ch2 = %c, str = %s", x, y, ch1, ch2, str);
    _getch();
}

//void printRecData(int foffset) {
//    union MyUnion
//    {
//        int id;
//        char str[30];
//        int age;
//        char dept[15];
//        char ch;
//    }myunion;
//
//    int dlen = 0, prev_len = 0;
//    for (int i = 0; i <= 5; i++) {
//        switch (ftype[i])
//        {
//        case 'L':
//            dlen = sizeof(int);
//            fwidth[i] = dlen;
//            prev_len = prev_len + fwidth[i];
//            break;
//
//        case 'I':
//            dlen = sizeof(int);
//            fwidth[i] = dlen;
//            prev_len = prev_len + fwidth[i];
//            break;
//
//        case 'T':
//            dlen = fwidth[i]; // +(4 - (fwidth[i] % 4));
//            fwidth[i] = dlen;
//            prev_len = prev_len + fwidth[i];
//            break;
//
//        case 'C':
//            dlen = 4;
//            fwidth[i] = 1;
//            prev_len = prev_len + fwidth[i];
//            break;
//
//        case 'P':
//            //dlen = prev_len + (4 + (prev_len % 4));
//            dlen = (4 - (prev_len % 4));
//            fwidth[i] = dlen;
//            break;
//
//        default:
//            break;
//        }
//    }
//
//    int prev = 0;
//    for (int i = 0; i <= 4; i++) {
//        foffset[i] = prev;
//        prev = prev + fwidth[i];
//    }
//
//    for (int i = 0; i <= 4; i++) {
//        switch (i) {
//        case 0:
//            myunion.id = 1001;
//            //foffset[i] = 0;
//            storeData(sdata, &myunion.id, sizeof(int), foffset[i]);
//            break;
//
//        case 1:
//            strcpy(myunion.str, "Mr. X");
//            //dlen = fwidth[i] + (4 - (fwidth[i] % 4));
//            //foffset[i] = foffset[i - 1] + dlen;
//            storeData(sdata, myunion.str, fwidth[i], foffset[i]);
//            break;
//
//        case 2:
//            myunion.age = 40;
//            //dlen = sizeof(int);
//            //foffset[i] = foffset[i - 1] + dlen;
//            storeData(sdata, &myunion.age, fwidth[i], foffset[i]);
//            break;
//
//        case 3:
//            strcpy(myunion.dept, "IT");
//            //dlen = fwidth[i] + (4 - (fwidth[i] % 4));
//            //foffset[i] = foffset[i - 1] + dlen;
//            storeData(sdata, myunion.dept, fwidth[i], foffset[i]);
//            break;
//
//        case 4:
//            myunion.ch = 'Y';
//            //dlen = 4;
//            //foffset[i] = foffset[i - 1] + dlen;
//            storeData(sdata, &myunion.ch, fwidth[i], foffset[i]);
//            break;
//        }
//    }
//
//
//    void* pdata;
//    for (int i = 0; i <= 4; i++) {
//        switch (i) {
//        case 0:
//            pdata = ((char*)sdata + foffset[i]);
//            printf("ID :: %d ", *(int*)pdata);
//            break;
//
//        case 1:
//            pdata = ((char*)sdata + foffset[i]);
//            printf("Name :: %s ", (char*)pdata);
//            break;
//
//        case 2:
//            pdata = ((char*)sdata + foffset[i]);
//            printf("Age :: %d ", *(char*)pdata);
//            break;
//
//        case 3:
//            pdata = ((char*)sdata + foffset[i]);
//            printf("Dept :: %s ", (char*)pdata);
//            break;
//
//        case 4:
//            pdata = ((char*)sdata + foffset[i]);
//            printf("Perm_Emp :: %c ", *(char*)pdata);
//            break;
//        }
//    }
//}

//void storeData(void* dst, void* src, int len, int offset) {
//    memcpy((char *)dst + offset, src, len);
//}

int freeMem(MAIN* tmain) {
    int counter = 1;
    if (tmain == NULL || tmain->start == NULL) {
        return 0;
    }

    LISTS *tlist = tmain->start;
    LISTS *temp = NULL;

    while (1) {
        if (tlist ==  (LISTS *) NULL) {
            //free(tlist->data);
            //free(tlist);
            break;
        }

        temp = tlist->next;
        free(tlist->data);
        free(tlist);

        tlist = temp;

        counter++;
    }

    //free(tmain);
    return 1;
}


//LISTS *addToList(MAIN *tmain, void *data, int len) {
//     
//    LISTS* tlist = (LISTS*)calloc(1, sizeof(LISTS));
//
//
//    if (tmain->start == NULL) {
//        tmain->start = tlist;
//        tmain->end = tlist;
//        tlist->next = NULL;
//    }
//    else {
//        void* tend;
//        tend = tmain->end;
//
//        tmain->end->next = tlist;
//        tmain->end = tlist;
//        tlist->next = NULL;
//    }
//
//    tlist->data = calloc(1, len);
//
//    memcpy(tlist->data, data, len);
//
//    return tlist;
//}


//void copyStr(char* src, char* dst) {
//    while (*src != NULL) {
//        *dst++ = *src++;
//    }
//}
//
//int strLen(char* src_str) {
//    char *temp = src_str;
//    if (*temp) {
//        while (*(++temp)) {}
//    }
//
//
//    return (temp - src_str);
//}
//
//
//void strAppend(char *src, char* dst) {
//    while (*(++dst));
//    *dst++ = ' ';
//    while (*src) {
//        *dst++ = *src++;
//    }
//
//    *dst = NULL;
//}
//
//char  *strToUpper(char* src) {
//    char *temp = src;
//    while (*src) {
//        if (*src >= 97 && *src <= 122) {
//            *src = *src - 32;
//        }
//        src++;
//    }
//
//    return temp;
//}
//
//char* strToLower(char* src) {
//    char* temp = src;
//    while (*src) {
//        if (*src >= 65 && *src <= 90) {
//            *src = *src + 32;
//        }
//        src++;
//    }
//
//    return temp;
//}
//
//char *strProperCase(char* src) {
//    char *temp = src;
//    bool FOUND = false , FIRST_CHAR = true;
//
//    while (*src) {
//        if (*(temp + 0) != 32  && FIRST_CHAR) {
//            *src = *src - 32;
//            src++;
//            FIRST_CHAR = false;
//        }
//
//        if (*src == 32) {
//            while (*src == 32) src++;
//            FOUND = true;
//        }
//        
//        if (FOUND) {
//            if (*src >= 97 && *src <= 122) {
//                *src = *src - 32;
//            }
//            FOUND = false;
//        }
//        src++;
//    }
//
//    int i = src - temp;
//
//    return temp;
//}
//
//char *strFind(char *src, char *search) {
//    char *temp_src = src;
//    char *temp_search = search;
//    bool FOUND = false;
//    int found_at = 0;
//    int search_len = 0;
//
//    while (*src) {
//        search_len = 0;
//
//        search = temp_search;
//        while (*search) {
//
//            if (*src == *search) {
//                src++;
//                search++;
//                search_len++;
//                FOUND = true;
//            }
//            else {
//                search = temp_search;
//                FOUND = false;
//                break;
//            }
//        }
//
//        found_at = (src - temp_src) - search_len;
//        if (FOUND) {
//            if ((temp_src == (src - search_len)) && (*src == 32 ||  *src == NULL)) {
//                break;
//            }
//            if (*(src - (search_len+1)) == 32 &&  (*src == 32 || *src == NULL)) {
//                break;
//            }
//        }
//        src++;
//        FOUND = false;
//    }
//    return NULL;
//}
//
//char toLower(char ch) {
//    char  ret_char = ch >= 65 && ch <= 90 ? ch + 32 : ch;
//
//    return ret_char;
//}
//
//char* striFind(char* src, char* search) {
//    char* temp_src = src;
//    char* temp_search = search;
//    char char_src = 32;
//    char char_search = 32;
//    bool FOUND = false;
//    int found_at = 0;
//    int search_len = 0;
//
//    while (*src) {
//        search_len = 0;
//
//
//        search = temp_search;
//        while (*search) {
//            char_src = toLower(*src);
//            char_search = toLower(*search);
//
//            if (char_src == char_search) {
//                src++;
//                search++;
//                search_len++;
//                FOUND = true;
//            }
//            else {
//                search = temp_search;
//                FOUND = false;
//                break;
//            }
//        }
//
//        found_at = (src - temp_src) - search_len;
//        if (FOUND) {
//            if ((temp_src == (src - search_len)) && (*src == 32 || *src == NULL)) {
//                break;
//            }
//            if (*(src - (search_len + 1)) == 32 && (*src == 32 || *src == NULL)) {
//                break;
//            }
//        }
//        src++;
//        FOUND = false;
//    }
//    return NULL;
//}
//
//
//char* strMove(char *src, int len) {
//    int str_len = strLen(src);
//    int pos = 0;
//    char *temp = src;
//    char *tstr = (char *) calloc(str_len+2, sizeof(char));
//
//    while (*src) {
//        *(tstr + pos) = *src;
//        src++;
//        pos++;
//    }
//    
//    *(tstr + pos) = NULL;
//
//    src = temp;
//
//    pos = 0;
//    while (pos < len) { 
//        *(src + pos) = '*';
//        pos++;
//    }
//
//    while (*tstr) {
//        *(src + pos) = *tstr;
//        pos++;
//        tstr++;
//    }
//
//
//    return temp;
//}
