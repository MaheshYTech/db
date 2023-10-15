#pragma once

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "std_define.h"

void        copyStr(char* src, char* dst);
int         strLen(char* src_str);
void        strAppend(char* src, char* dst);
char*       strToUpper(char* src);
char*       strToLower(char* src);
char*       strProperCase(char* src);
char*       strFind(char* src, char* search);
char*       striFind(char* src, char* search);
char        toLower(char ch);
char*       strMove(char* src, int len);
int         skipSpace(char* src);
void        removeChar(char* src, char ch);
void        blankMem(void* mem, int length);
void        myMemCopy(char* src, char* dst, int len);

void myMemCopy(char* src, char* dst, int len)
{
    while (len--) {
        *(dst++) = *(src++);
    }

    *(dst) = NULL;
}


void copyStr(char* src, char* dst) {
    while (*src != NULL) {
        *dst++ = *src++;
    }
}

int strLen(char* src_str) {
    char* temp = src_str;
    if (*temp) {
        while (*(++temp)) {}
    }


    return (temp - src_str);
}


void strAppend(char* src, char* dst) {
    while (*(++dst));
    *dst++ = ' ';
    while (*src) {
        *dst++ = *src++;
    }

    *dst = NULL;
}

char* strToUpper(char* src) {
    char* temp = src;
    char ch = ' ';
    int i = 0;
    while (*src) {
        ch = *src;
        if (ch >= 97 && ch <= 122) {
            *src = (ch - 32); // (*src - 32);
        }
        src++;
    }

    return temp;
}

char* strToLower(char* src) {
    char* temp = src;
    while (*src) {
        if (*src >= 65 && *src <= 90) {
            *src = *src + 32;
        }
        src++;
    }

    return temp;
}

char* strProperCase(char* src) {
    char* temp = src;
    bool FOUND = false, FIRST_CHAR = true;

    //if (*src >= 97 && *src <= 122) {
    //    *src = *src - 32;
    //}

    //src++;

    while (*src) {
        if (*(temp + 0) != 32 && FIRST_CHAR) {
            *src = *src - 32;
            src++;
            FIRST_CHAR = false;
        }

        if (*src == 32) {
            while (*src == 32) src++;
            FOUND = true;
        }

        if (FOUND) {
            if (*src >= 97 && *src <= 122) {
                *src = *src - 32;
            }
            FOUND = false;
        }
        src++;
    }

    int i = src - temp;

    return temp;
}

char* strFind(char* src, char* search) {
    char* temp_src = src;
    char* temp_search = search;
    bool FOUND = false;
    int found_at = 0;
    int search_len = 0;

    while (*src) {
        search_len = 0;

        search = temp_search;
        while (*search) {

            if (*src == *search) {
                src++;
                search++;
                search_len++;
                FOUND = true;
            }
            else {
                search = temp_search;
                FOUND = false;
                break;
            }
        }

        found_at = (src - temp_src) - search_len;
        if (FOUND) {
            if ((temp_src == (src - search_len)) && (*src == 32 || *src == NULL)) {
                break;
            }
            if (*(src - (search_len + 1)) == 32 && (*src == 32 || *src == NULL)) {
                break;
            }
        }
        src++;
        FOUND = false;
    }
    if (FOUND) {
        return (src);
    }
    else {
        return NULL;
    }

}

char toLower(char ch) {
    char  ret_char = ch >= 65 && ch <= 90 ? ch + 32 : ch;

    return ret_char;
}

char* striFind(char* src, char* search) {
    char* temp_src = src;
    char* temp_search = search;
    char char_src = 32;
    char char_search = 32;
    bool FOUND = false;
    int found_at = 0;
    int search_len = 0;

    while (*src) {
        search_len = 0;


        search = temp_search;
        while (*search) {
            char_src = toLower(*src);
            char_search = toLower(*search);

            if (char_src == char_search) {
                src++;
                search++;
                search_len++;
                FOUND = true;
            }
            else {
                search = temp_search;
                FOUND = false;
                break;
            }
        }

        found_at = (src - temp_src) - search_len;
        if (FOUND) {
            if ((temp_src == (src - search_len)) && (*src == 32 || *src == NULL)) {
                break;
            }
            if (*(src - (search_len + 1)) == 32 && (*src == 32 || *src == NULL)) {
                break;
            }
        }
        src++;
        FOUND = false;
    }
    return NULL;
}


char* strMove(char* src, int len) {
    int str_len = strLen(src);
    int pos = 0;
    char* temp = src;
    char* tstr = (char*)calloc((str_len + 2), sizeof(char));

    //strcpy_s(tstr, str_len, src );

    while (*src) {
        *(tstr + pos) = *src;
        src++;
        pos++;
    }

    *(tstr + pos) = NULL;

    src = temp;

    pos = 0;
    while (pos < len) {
        *(src + pos) = '*';
        pos++;
    }

    while (*tstr) {
        *(src + pos) = *tstr;
        pos++;
        tstr++;
    }


    return temp;
}

int skipSpace(char* src) {
    char* temp = src;

    while (*src == ' ') {
        if (*src == NULL) {
            break;
        }
        src++;
    }

    return src - temp;
}

void  removeChar(char* src, char ch) {
    int i = 0;
    int n = 0;
    while (*(src + i)) {
        if (*(src + i) != ch) {
            *(src + n) = *(src + i);
            n++;
        }
        i++;
    }

    *(src + n) = NULL;
}

void blankMem(void* mem, int length) {
    for (int i = 0; i < length; i++) {
        *((CHPTR mem) + i) = NULL;
    }
}