#pragma once
#include <stdlib.h>
#include <stdio.h>
typedef int BOOL;
#define TRUE (BOOL)1
#define FALSE (BOOL)0

typedef enum
{
  OC_OK,
  OC_YES,
  OC_NO,
  E_MEMALLOCATION_ERROR,
  E_NULL_REF,
  E_INCORRECT_COMMAND_FORMAT,
  E_UNCLASSIFIED
} OUT_CODE;

typedef struct Set_t_ Set_t;
typedef struct SetList_t_ SetList_t;

//OK, ERROR
OUT_CODE SetListGet(SetList_t** setList_p);

//OK, ERROR
OUT_CODE SetListCount(SetList_t* setList, int* count_p);

//YES, NO, ERROR
OUT_CODE SetListAdd(SetList_t* setList, Set_t* set);

//YES, NO, ERROR
OUT_CODE SetListFind(SetList_t* setList, Set_t** outSet, const char* name);

//YES, NO, ERROR
OUT_CODE SetListRemove(SetList_t* setList, Set_t** outSet, const char* name);

//OK, ERROR
OUT_CODE SetListFree(SetList_t* setList);

//YES, NO, ERROR
OUT_CODE SetListPrint(SetList_t* setList, FILE* stream, const char* setFormat);

//YES, NO, ERROR
OUT_CODE SetListPrintAll(SetList_t* setList, FILE* stream, const char* setFormat, const char* itemsStart, const char* itemsFormat, const char* itemsEnd);

//OK, ERROR
OUT_CODE SetGet(Set_t** const output, const char* name);

//OK, ERROR
OUT_CODE SetFree(Set_t* set);

//OK, ERROR
OUT_CODE SetName(Set_t* set, char** name_p);

//OK, ERROR
OUT_CODE SetRename(Set_t* set, const char* name);

//OK, ERROR
OUT_CODE SetCardinality(Set_t* set, int* cardinality_p);

//OK, ERROR
OUT_CODE SetGetCopy(Set_t* setFrom, Set_t** setTo_p, const char* name);

//YES, NO, ERROR
OUT_CODE SetHasItem(Set_t* set, const char* string);

//YES, NO, ERROR
OUT_CODE SetAddItem(Set_t* set, const char* string);

//YES, NO, ERROR
OUT_CODE SetRemoveItem(Set_t* set, const char* string);

//OK, ERROR
OUT_CODE SetPrintItemList(Set_t* set, FILE* stream, const char* setFormat, const char* itemsStart, const char* itemsFormat, const char* itemsEnd);

//OK, ERROR
OUT_CODE SetBinOper(Set_t* setA, Set_t* setB, Set_t** output, const char* name, BOOL onlyA, BOOL AnB, BOOL onlyB);

//OK, ERROR
OUT_CODE SetUnion(Set_t* setA, Set_t* setB, Set_t** output, const char* name);

//OK, ERROR
OUT_CODE SetIntersection(Set_t* setA, Set_t* setB, Set_t** output, const char* name);

//OK, ERROR
OUT_CODE SetDifference(Set_t* setA, Set_t* setB, Set_t** output, const char* name);

//OK, ERROR
OUT_CODE SetSymDifference(Set_t* setA, Set_t* setB, Set_t** output, const char* name);

//YES, NO, ERROR
OUT_CODE SetIsInclusion(Set_t* setA, Set_t* setB);
