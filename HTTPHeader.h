#pragma once

#include <stdio.h>
#include <ctype.h>

#include "./libs/Map.h"
#include "./libs/Entries.h"

// Header configuration default values
#define HHDR_DEFAULT_KEY_LENGTH    (unsigned char)64
#define HHDR_DEFAULT_VALUE_SIZE    (unsigned short)128
#define HHDR_DEFAULT_MAP_INIT_SIZE (unsigned char)10

// Header characters.
#define HHDR_BODY_SEPARATOR  ':'
#define HHDR_WHITE_SPACE     ' '
#define HHDR_NEW_LINE        '\n'
#define HHDR_CARRIAGE_RETURN '\r'

// Types constants.
#define HHDR_TRUE  (unsigned char)1
#define HHDR_FALSE (unsigned char)0

// Errors/Succes constants.
#define HHDR_ERROR_ILLEGAL_KEY_LENGTH (unsigned char)0
#define HHDR_ERROR_ILLEGAL_VALUE_SIZE (unsigned char)1
#define HHDR_ERROR_MEMALLOCATION      (unsigned char)2
#define HHDR_SUCCES                   (unsigned char)3

#define HHDR_HEADER_HEAD_KEYS_LENGTH (unsigned char)3

/*===========================================*/
typedef unsigned char  HHDR_Bool;
/*===========================================*/
typedef unsigned char  HHDR_Return_Code;
/*===========================================*/
typedef const char*    HHDR_CString;
typedef char*          HHDR_String;
typedef char           HHDR_Char;
/*===========================================*/
typedef unsigned char  HHDR_UChar;
/*===========================================*/
typedef unsigned short HHDR_UShort;
/*===========================================*/
typedef void HHDR_None;
/*===========================================*/

typedef struct HHDR_Config {
	HHDR_UChar key_length;
	HHDR_UShort value_size;
	HHDR_UShort map_size;
} HHDR_Config;

typedef struct HHDR_Raw {
	HHDR_CString string;
	HHDR_Config* config;
	HHDR_UShort length;
} HHDR_Raw;

HHDR_Return_Code hhdr_parse(HHDR_Raw raw, Map* map);
HHDR_Return_Code hhdr_parse_head(HHDR_Raw raw, Map* map, HHDR_UShort* skiped);
HHDR_Return_Code hhdr_parse_body(HHDR_Raw raw, Map* map);
HHDR_Return_Code hhdr_delete_header(HHDR_CString header);

HHDR_String hhdr_create(Entries entries);