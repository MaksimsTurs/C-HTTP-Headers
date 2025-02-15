#pragma once

#include <stdlib.h>
#include <string.h>

// Error constants.
#define ENTRY_ERROR_MEMALLOC      (unsigned char)0
#define ENTRY_ERROR_INVALID_PARAM (unsigned char)2
#define ENTRY_ERROR_NULL_PTR      (unsigned char)3
#define ENTRY_SUCCES              (unsigned char)1

/*===========================================*/
typedef unsigned char Ent_Return_Code;
/*===========================================*/
typedef char* Ent_String;
typedef const char* Ent_CString;
/*===========================================*/
typedef void* Ent_Any;
/*===========================================*/
typedef unsigned int Ent_UInt;
/*===========================================*/
typedef unsigned short Ent_UShort;
/*===========================================*/
typedef unsigned long long Ent_ULLong;
/*===========================================*/

typedef struct Entrie {
	Ent_String key;
	Ent_Any value;
} Entrie;

typedef struct Entries {
	Entrie** items;
	Ent_UInt size;
} Entries;

Ent_Return_Code entrie_push(Entries* entries, Ent_CString key, Ent_Any value, Ent_UShort value_size);
Ent_Return_Code entrie_pop(Entries entries);
Ent_Return_Code entrie_delete(Entries entries);