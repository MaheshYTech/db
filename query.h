#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "db.h"
#include "mylib.h"
#include "std_define.h"



bool WHERE_COND_MISSING = true;

typedef struct WHERECOND {
    int     findx;  // Field Index  Example Age
    char    fname[15];
    char    wcond[4]; // < <= > >= = !=
    char    condValue[50];   // Example 30
    int     wresult;  // true 1 or 0 False
    char    logical[4]; // AND OR
    int     logResult;  
}WHERE_CONDS;

WHERECOND wherePart[5] = { NULL };

int         getSelectField(char* sql);
int         getWhereCondition(char* where_str);
int         getWhereConditionValue(char* where_str);
int         getSeleFieldStartPos(char* seleArr, int* sele_field_count);
int         processWhere(char* sqlStr);
void        initWhereVarValue();
int         getWhereField(char* where_str);
bool        verifyWhereCondition(char* data);
int         compareResult(char* conditionStr, float comp_result);
int         openDataBase(char *sql, char* file_name);
int         printQueryResult(MAIN* tmain, DB_MAIN* db_main);
int         getSqlStatement(char *sql);
//void        startSql(int MODE);

int getSqlStatement(char* sql) {
    system("cls");
    fseek(stdin, 0, SEEK_END);
    printf("\n\n\n\n");
    printf(" Enter Sql Satetment :: \n");
    scanf("%[^\n]s", sql);

    int len = strlen(sql);
    sql[len + 1] = 32;
    sql[len + 2] = NULL;
    return 1;
}


int openDataBase(char *sql, char *file_name) {
    getDataBaseFile(sql, file_name);
    if (strlen(file_name) <= 0) {
        return 0;
    }

    if (fp != NULL)
    {
        fclose(fp);
    }

    fp = fopen(file_name, "rb+");
    if (fp == NULL) {
        return 1;
    }
}




int getSelectField(char* sql) {
    int seleWidth = 0, sele_field_count = 0, result = 0 , findx = 0;
    char* seleStr = strFind(sql, CHPTR "select");
    char* from = strFind(seleStr, CHPTR "from");
    seleWidth = ((from - 4) - seleStr);
    char str[512] = { NULL };
    memcpy(str, seleStr, seleWidth);
    str[seleWidth + 1] = NULL;
    result = getSeleFieldStartPos(str, &sele_field_count);
    seleWidth = 0;
    if (result == NULL) {
        return NULL;
    }else {
        return 1;
    }
}

int getSeleFieldStartPos(char* sele_Arr, int* sele_field_count)
{
    int  sepCount = 0, i = 0, n = 0;
    char ch = ' ', field_name[30] = { NULL };
    int len = strlen(sele_Arr) + 5;
    char* seleArr = NULL;
    bool FOUND = true;
    int findResult = 0, prev_pos = 0, chCount = 0 , selectAllCount = 0;

    seleArr = (char*) calloc(1, len);
    strcpy(seleArr, sele_Arr);

    while (1) {
        if (*(seleArr + i) == ',') {
            n++;
        }
        else if(*(seleArr + i) == '*') {
            for (int indx = 0; indx <= MAX_FIELD; indx++) {
                if (Fields[indx].fname[0] == NULL) {
                    break;
                }
                n++;
            }
        }if (*(seleArr + i) == NULL) {
            n++;
            break;
        }
        i++;
    }

    n = n + 2;
    seleFields = (SELECTFIELDS*) calloc((n), sizeof(SELECTFIELDS));
    n = 0;
    i = 0;
    while (*(seleArr + i)) {
        ch = *(seleArr + i);
        if (ch != 32) {
            *(seleArr + n) = ch;
            n++;
        }
        i++;
    }


    *(seleArr + n) = NULL;
    //printf("\n** %s **\n", seleArr);

    i = 0;
    chCount = 0;
    FOUND = true;
    while (1) {
        if (*(seleArr + i) == '*') {
            for (int indx = 0; indx <= MAX_FIELD; indx++) {
                if (Fields[indx].ftype == NULL) {
                    break;
                }
                seleFields[selectAllCount].Fields = Fields[indx];
                //sepCount++;
                selectAllCount++;
            }
        }else if (*(seleArr + i) == ',' || *(seleArr + i) == NULL) {
            field_name[chCount] = NULL;
            findResult = findFieldName(Fields, field_name);
            if (findResult >= 0) {
                seleFields[selectAllCount + sepCount].Fields = Fields[findResult];
            }else {
                if (sepCount == 0) {
                    FOUND = true;
                }
                else {
                    FOUND = false;
                }
                
                break;
            }
            sepCount++;
            if (*(seleArr + i) == NULL) {
                break;
            }
            chCount = 0;
        }else {
            field_name[chCount] = *(seleArr + i);
            chCount++;
        }
        i++;
    }

    seleFields[selectAllCount + sepCount] = { NULL };


    free(seleArr);
    if (!FOUND) {
        return NULL;
    }
    else {
        return 1;
    }

}

int printQueryResult(MAIN *tmain, DB_MAIN *db_main) {
    LISTS *tlist =  tmain->start;
    char* data = NULL;
    char field_format[100] = {NULL};
    char strData[50];
    int WHERE_RESULT = 0 , slno = 0;
    

    while (1) {
        if (tlist == NULL) {
            break;
        }
        data = (char*)tlist->data;

        if (!WHERE_COND_MISSING) {
            WHERE_RESULT = verifyWhereCondition(data);
        }
        else {
            WHERE_RESULT = 1;
        }

        int i = 0;
        if (WHERE_RESULT) {
            printf("\n\t%-5d" , slno + 1);
            while (1) {
                if (seleFields[i].Fields.fname[0] == NULL) {
                    break;
                }
                FIELDS f = seleFields[i].Fields;
                switch (seleFields[i].Fields.ftype) {
                case 'T':
                    if (strlen(data + f.foffset) >= 30) {
                        strncpy(strData, (data + f.foffset), 30);
                        *(strData + 30) = NULL;
                    }
                    else {
                        strncpy(strData, (data + f.foffset), 30);
                    }
                    sprintf(field_format, "\t%%-%ds", f.fwidth <= 30 ? f.fwidth : 30); // f.fwidth >= 30 ? f.fwidth : 30); // seleFields[i].Fields.fwidth);
                    printf(field_format, strData); // (data + f.foffset));
                    break;

                case 'I':
                    printf("%15d ", *(int*)(data + f.foffset)); // seleFields[i].Fields.foffset));
                    break;

                case 'F':
                    printf("%15.2f", *(float*)(data + f.foffset)); // seleFields[i].Fields.foffset));
                    break;

                case 'C':
                    //sprintf(strArray, "\t%%-5s");;
                    printf("%%-5s", *(char *)(data + f.foffset)); // seleFields[i].Fields.foffset));
                    break;
                }

                i++;
            }
            slno++;
        }

        tlist = tlist->next;
    }

    _getch();
    return 0;
}

void initWhereVarValue() {
    WHERE_COND_MISSING = false;
    for (int i = 0; i < 5; i++) {
        wherePart[i] = { NULL };
    }
}

int processWhere(char* sqlStr) {
    int i = 0, intResult = 0, windx = 0;
    bool LOGIC_FOUND = false;
    char strCond[4] = { NULL };
    char* tempWhere = NULL;
    char* whereStr = strFind(sqlStr, CHPTR "where");

    initWhereVarValue();

    if (whereStr == NULL) {
        WHERE_COND_MISSING = true;
        return -1;
    }

    tempWhere = whereStr;

    while (1) {
        LOGIC_FOUND = false;
        intResult = skipSpace(tempWhere);
        tempWhere = intResult + tempWhere;
        int fieldIndex = getWhereField(tempWhere);

        i = 0;
        FIELDS f = { NULL };

        if (fieldIndex >= 0) {
            f = Fields[fieldIndex];
            wherePart[windx].findx = fieldIndex;
            strcpy_s(wherePart[windx].fname, CHPTR f.fname);
            //printf("\nField Name %s", f.fname);
            i = strlen(f.fname);
        }
        else {
            return NULL;
        }

        tempWhere = tempWhere + i;
        intResult = skipSpace(tempWhere);
        tempWhere = tempWhere + intResult;

        intResult = getWhereCondition(tempWhere);
        strncpy_s(strCond, tempWhere, intResult);
        strCond[intResult] = NULL;
        tempWhere = tempWhere + intResult;

        strcpy_s(wherePart[windx].wcond, strCond);
        intResult = skipSpace(tempWhere);
        tempWhere = tempWhere + intResult;
        intResult = getWhereConditionValue(tempWhere);
        strncpy_s(wherePart[windx].condValue, tempWhere, intResult);

        tempWhere = tempWhere + intResult;

        intResult = skipSpace(tempWhere);
        tempWhere = tempWhere + intResult;
        char* logicResult = strFind(tempWhere, CHPTR "and");
        if (strFind(tempWhere, CHPTR "and")) {
            strncpy_s(wherePart[windx].logical, tempWhere, 3);
            tempWhere = tempWhere + 3;
            LOGIC_FOUND = true;
        }
        else if (strFind(tempWhere, CHPTR "or")) {
            strncpy_s(wherePart[windx].logical, tempWhere, 2);
            tempWhere = tempWhere + 3;
            LOGIC_FOUND = true;
        }

        if (1) {
            if (Fields[wherePart[windx].findx].ftype == 'T') {
                removeChar(wherePart[windx].condValue, 39);
            }
        }

        if (!LOGIC_FOUND) {
            break;
        }
        else {
            //printf("5 %s", wherePart[windx].logical);
        }
        windx++;
    }

    wherePart[windx + 1] = { NULL };
    return 1;
}

int getWhereField(char* where_str) {
    char field_name[30] = { NULL };
    int i = 0;

    while (1) {
        if (*where_str == NULL || *where_str != 32) {
            break;
        }
        where_str++;
    }

    while (1) {
        if (*(where_str + i) == 32 || *(where_str + i) == NULL) {
            break;
        }
        field_name[i] = *(where_str + i);
        i++;
    }

    field_name[i] = NULL;

    int fieldIndex = findFieldName(Fields, field_name);

    return fieldIndex;
}

int getWhereCondition(char* where_str) {
    char* temp = where_str;
    int i = 0;
    while (*where_str != 32) {
        if (*where_str == NULL) {
            break;
        }

        i++;
        where_str++;
    }

    //while (1) {
    //    
    //}
    return where_str - temp;
}

int getWhereConditionValue(char* where_str) {
    char* temp = where_str;
    int i = 0;
    while (*where_str != 32) {
        if (*where_str == NULL) {
            break;
        }

        i++;
        where_str++;
    }

    return where_str - temp;
}

bool verifyWhereCondition(char* data) {
    int i = 0, condCount = 0;
    while (1) {
        WHERE_CONDS w = wherePart[i];
        FIELDS f = Fields[w.findx];
        int intVal_1 = 0, intVal_2 = 0;
        float floaVal_1 = 0, floaVal_2 = 0, COMP_RESULT = 0;
        char charVal_1 = ' ', charVal_2 = ' ';

        if (w.fname[0] == NULL) {
            break;
        }if (f.ftype == 'T') {
            COMP_RESULT = strcmp((char*)data + f.foffset, w.condValue);
        }
        else if (f.ftype == 'I') {
            intVal_1 = INTPTRVAL(CHPTR data + f.foffset);
            intVal_2 = atoi(w.condValue);
            COMP_RESULT = intVal_1 - intVal_2;
        }
        else if (f.ftype == 'F') {
            floaVal_1 = FLOATPTRVAL(CHPTR data + f.foffset);
            floaVal_2 = atof(w.condValue);
            COMP_RESULT = floaVal_1 - floaVal_2;
        }
        else if (f.ftype == 'C') {
            charVal_1 = *(char*)(data + f.foffset);
            COMP_RESULT = strncmp(&charVal_1, w.condValue, 1);
        }

        wherePart[i].wresult = compareResult(w.wcond, COMP_RESULT);
        i++;
    }

    condCount = i;
    i = 0;
    int prevResult = -1, presResult = 0, logResult = -1;
    char prevLogic[4] = "AND";

    while (i < condCount) {
        WHERE_CONDS w = wherePart[i];
        if (w.findx == NULL) {
            break;
        }

        if (!strcmp(w.logical, "and")) {
            if (prevResult != -1) {
                presResult = w.wresult;
                w.logResult = (prevResult && presResult) ? 1 : 0;
                logResult = w.logResult;
            }
            else {
                presResult = w.wresult;
                w.logResult = w.wresult;
                logResult = w.logResult;
            }
            prevResult = presResult;
            strcpy(prevLogic, "and");
        }else if (!strcmp(w.logical, "or")) {
            if (prevResult != -1) {
                presResult = w.wresult;
                w.logResult = (prevResult || presResult) ? 1 : 0;
                logResult = w.logResult;
            }
            else {
                presResult = w.wresult;
                w.logResult = w.wresult;
                logResult = w.logResult;
            }
            prevResult = presResult;
            strcpy(prevLogic, "or");
        }else {
            if (prevResult != -1) {
                presResult = w.wresult;

                if (!strcmp(prevLogic, "or")) {
                    w.logResult = (prevResult || w.wresult) ? 1 : 0;
                }else if (!strcmp(prevLogic, "and")) {
                    w.logResult = (prevResult && w.wresult) ? 1 : 0;
                }

                logResult = w.logResult;
            }
            else {
                presResult = w.wresult;
                w.logResult = w.wresult;
                logResult = w.logResult;
            }
            prevResult = presResult;
        }
        i++;
    }

    return logResult;
}

int compareResult(char* conditionStr, float comp_result) {
    int finalCompResult = 0, i = 0;
    char* condArray[] = {
        CHPTR "<" ,
        CHPTR "<=" ,
        CHPTR ">" ,
        CHPTR ">=" ,
        CHPTR "=" ,
        NULL
    };

    bool COND_FOUND = false;
    while (condArray[i] != NULL) {
        if (!strcmp(condArray[i], conditionStr)) {
            COND_FOUND = true;
            break;
        }
        i++;
    }

    //if (i > 0) i--;

    switch (i) {
    case 0: // <
        if (comp_result < 0) {
            finalCompResult = 1;
        }
        else {
            finalCompResult = 0;
        }
        break;

    case 1: // <=
        if (comp_result <= 0) {
            finalCompResult = 1;
        }
        else {
            finalCompResult = 0;
        }
        break;

    case 2: // >
        if (comp_result > 0) {
            finalCompResult = 1;
        }
        else {
            finalCompResult = 0;
        }

        break;

    case 3: // >=
        if (comp_result >= 0) {
            finalCompResult = 1;
        }
        else {
            finalCompResult = 0;
        }
        break;

    case 4: // ==
        if (comp_result == 0) {
            finalCompResult = 1;
        }
        else {
            finalCompResult = 0;
        }
        break;
    }

    return finalCompResult;
}