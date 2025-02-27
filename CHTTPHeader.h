#pragma once

/*=======================================================================*/

#include "./include/CString.h"

/*=======================================================================*/

typedef unsigned char      CHHDR_Ret_Code;
typedef unsigned char      CHHDR_Bool;
typedef unsigned char      CHHDR_UChar;
typedef char               CHHDR_Char;

typedef void*              CHHDR_Any;
typedef void               CHHDR_None;

typedef unsigned int       CHHDR_UInt;

typedef unsigned short     CHHDR_UShort;

typedef unsigned long long CHHDR_ULLong;
typedef long long          CHHDR_LLong;

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
#define CHHDR_ERROR_INVALID_HTTP_TYPE     (CHHDR_UChar)0
#define CHHDR_ERROR_INVALID_METHOD        (CHHDR_UChar)1
#define CHHDR_ERROR_INVALID_STATE         (CHHDR_UChar)2
#define CHHDR_ERROR_INVALID_PTR           (CHHDR_UChar)3
#define CHHDR_ERROR_BUFFER_OVERFLOW       (CHHDR_UChar)4
#define CHHDR_SUCCESS                     (CHHDR_UChar)5
// Parser states.
#define CHHDR_STATE_START                 (CHHDR_UChar)0
#define CHHDR_STATE_KEY                   (CHHDR_UChar)1
#define CHHDR_STATE_VALUE                 (CHHDR_UChar)2
#define CHHDR_STATE_BETWEEN_KEY_AND_VALUE (CHHDR_UChar)3
#define CHHDR_STATE_END                   (CHHDR_UChar)4

#define CHHDR_RAW_HEADER_APPEND           (CHHDR_UChar)1
#define CHHDR_RAW_HEADER_RESET            (CHHDR_UChar)2

#define CHHDR_FAIL_IF(condition, error_code) do { \
	if(condition) return error_code;                \
} while(0);
#define CHHDR_PEAK_TOKEN(str, offset) *(str + offset)
#define CHHDR_LIKELY(condition)       __builtin_expect(!!(condition), CHHDR_TRUE)
#define CHHDR_UNLIKELY(condition)     __builtin_expect(!!(condition), CHHDR_FALSE)

/*=======================================================================*/

typedef void (*CHHDR_on_head)(CHHDR_Any user_data, CSTR_Slice slice_1, CSTR_Slice slice_2, CSTR_Slice slice_3);
typedef void (*CHHDR_on_key_value)(CHHDR_Any user_data, CSTR_Slice key, CSTR_Slice value);

typedef struct {
	const CHHDR_on_key_value on_key_value;
	const CHHDR_on_head on_head;
	const CHHDR_UChar type;
	CHHDR_Any user_data;
	CHHDR_Char* string;
	CHHDR_UShort length;
} CHHDR_Header;

/*=======================================================================*/

CHHDR_Ret_Code chhdr_parse(CHHDR_Header header);
CHHDR_Ret_Code chhdr_append(CHHDR_Char* buff, CHHDR_UShort buff_size, const CHHDR_Char* key, CHHDR_UShort key_len, CHHDR_Char* value, CHHDR_UShort value_len, CHHDR_UChar flag);

/*=======================================================================*/