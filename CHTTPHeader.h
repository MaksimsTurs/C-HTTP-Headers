#pragma once

/*=======================================================================*/

#include "./include/CString.h"

/*=======================================================================*/

typedef unsigned char      CHHDR_Ret_Code;
typedef unsigned char      CHHDR_Bool;
typedef unsigned char      CHHDR_UChar;
typedef char               CHHDR_Char;
typedef char*              CHHDR_Char_Ptr;

typedef void*              CHHDR_Any;
typedef void               CHHDR_None;

typedef unsigned int       CHHDR_UInt;

typedef unsigned short     CHHDR_UShort;

typedef unsigned long long CHHDR_Hash;
typedef unsigned long long CHHDR_ULLong;

typedef float              CHHDR_Float;

/*=======================================================================*/

// Header types constants.
#define CHHDR_HEADER_RESPONSE             (CHHDR_UChar)0
#define CHHDR_HEADER_REQUEST              (CHHDR_UChar)1
// Characters.
#define CHHDR_BODY_SEPARATOR              ':'
#define CHHDR_WHITE_SPACE                 ' '
#define CHHDR_NEW_LINE                    '\n'
#define CHHDR_CARRIAGE_RETURN             '\r'
#define CHHDR_NULL_TERMINATOR             '\0'
// Types constants.
#define CHHDR_TRUE                        (CHHDR_UChar)1
#define CHHDR_FALSE                       (CHHDR_UChar)0
// Errors/Succes constants.
#define CHHDR_ERROR_ILLEGAL_HTTP_TYPE     (CHHDR_UChar)3
#define CHHDR_ERROR_ILLEGAL_METHOD        (CHHDR_UChar)5
#define CHHDR_ERROR_ILLEGAL_STATE         (CHHDR_UChar)8
#define CHHDR_SUCCESS                     (CHHDR_UChar)12
// URL Parser states.
#define CHHDR_STATE_START                 (CHHDR_UChar)0
#define CHHDR_STATE_KEY                   (CHHDR_UChar)1
#define CHHDR_STATE_VALUE                 (CHHDR_UChar)2
#define CHHDR_STATE_BETWEEN_KEY_AND_VALUE (CHHDR_UChar)3

#define CHHDR_SAFE_CALL(error_code, success_code) do { \
	if(error_code != success_code) return error_code;    \
} while(0);
#define CHHDR_PEAK_TOKEN(str, offset) *(str + offset)

/*=======================================================================*/

typedef void (*CHHDR_on_head)(CSTR_Slice slice_1, CSTR_Slice slice_2, CSTR_Slice slice_3);
typedef void (*CHHDR_on_key_value)(CSTR_Slice key, CSTR_Slice value); 

typedef struct {
	const CHHDR_Char_Ptr string;
	const CHHDR_UChar type;
	const CHHDR_UShort length;
	const CHHDR_on_key_value on_key_value;
	const CHHDR_on_head on_head;
} CHHDR_Header;

/*=======================================================================*/

CHHDR_Ret_Code chhdr_parse(CHHDR_Header const header);

/*=======================================================================*/