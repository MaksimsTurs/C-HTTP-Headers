#pragma once
/*=======================================================================*/
#include <stdlib.h>	
#include <string.h>

#include "./SBString.h"
/*=======================================================================*/
typedef unsigned char      Ent_Return_Code;
typedef unsigned char      Ent_Bool;
typedef unsigned char      Ent_UChar;
typedef char*              Ent_String;

typedef void*              Ent_Any;

typedef unsigned int       Ent_UInt;

typedef unsigned short     Ent_UShort;

typedef unsigned long long Ent_ULLong;
/*=======================================================================*/
// Validation/restrictions constants.
#define ENT_KEY_MAX_LENGTH           (Ent_UChar)64
// Types constants.
#define ENT_TRUE                     (Ent_UChar)1
#define ENT_FALSE                    (Ent_UChar)0
// Error/success constants.
#define ENT_ERROR_INVALID_KEY_LENGTH (Ent_UChar)0
#define ENT_ERROR_INVALID_SIZE       (Ent_UChar)1
#define ENT_ERROR_INVALID_PTR        (Ent_UChar)2
#define ENT_ERROR_MEMALLOC           (Ent_UChar)3
#define ENT_SUCCESS                  (Ent_UChar)4
/*=======================================================================*/
typedef struct {
	Ent_String key;
	Ent_Any value;
} Entrie;

typedef struct {
	Entrie** items;
	Ent_UInt size;
} Entries;
/*=======================================================================*/
Ent_Return_Code ent_push(Entries* entries, const Ent_String key, const Ent_Any value, const Ent_UChar value_size);
Ent_Return_Code ent_pop(Entries* entries);
Ent_Return_Code ent_free(Entries* entries);
/*=======================================================================*/