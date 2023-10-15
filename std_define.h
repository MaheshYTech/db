#pragma once

#define  CHPTR         (char  *)
#define  CHPTRVAL     *(char  *)
#define  INTPTR        (int   *)
#define  INTPTRVAL    *(int   *)
#define  FLOATPTR      (float *)
#define  FLOATPTRVAL  *(float *)
#define  DBLPTR        (double *)
#define  DBLPTRVAL    *(double *)

#define  MAX_FIELD 32

#define  DATA_PAD 0

FILE* fp = NULL;

int fd = 0;

typedef struct {
    void* ret_adrs;
    int   ret_code;
}RET_RESULT;

typedef struct {
    int   recLengh;
    void  *data;
}REC;


RET_RESULT* ret_result = NULL;