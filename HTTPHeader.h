#pragma once
/*=======================================================================*/
#include <stdio.h>

#include "./libs/Map.h"
#include "./libs/Entries.h"
/*=======================================================================*/
// HTTP Headers constants
#define HHDR_HTTP_MIN_CODE (unsigned short)100
#define HHDR_HTTP_MAX_CODE (unsigned short)599
#define HHDR_HTTP_METHODS  { "POST", "GET", "PUT", "PATCH", "DELETE" }
#define HHDR_HTTP_VERSIONS { "HTTP/1.1", "HTTP/2", "HTTP/3" }

// Header configuration default values
#define HHDR_DEFAULT_KEY_LENGTH (unsigned char)30
#define HHDR_DEFAULT_VALUE_SIZE (unsigned short)64
#define HHDR_DEFAULT_MAP_SIZE   (unsigned char)6

// Header types constants.
#define HHDR_HEADER_RESPONSE (unsigned char)1
#define HHDR_HEADER_REQUEST  (unsigned char)2

// Header characters.
#define HHDR_BODY_SEPARATOR  ':'
#define HHDR_WHITE_SPACE     ' '
#define HHDR_NEW_LINE        '\n'
#define HHDR_CARRIAGE_RETURN '\r'

// Types constants.
#define HHDR_TRUE  (unsigned char)1
#define HHDR_FALSE (unsigned char)0

// Errors/Succes constants.
#define HHDR_ERROR_ILLEGAL_KEY_LENGTH   (unsigned char)0
#define HHDR_ERROR_ILLEGAL_VALUE_SIZE   (unsigned char)1
#define HHDR_ERROR_ILLEGAL_STATUS_CODE  (unsigned char)2
#define HHDR_ERROR_ILLEGAL_HEAD_TYPE    (unsigned char)3
#define HHDR_ERROR_ILLEGAL_METHOD       (unsigned char)4
#define HHDR_ERROR_ILLEGAL_HTTP_VERSION (unsigned char)6
#define HHDR_ERROR_INCORECT_SEQUENCE    (unsigned char)5
#define HHDR_ERROR_UNALLOWED_CHARACTER  (unsigned char)7
#define HHDR_ERROR_MEMALLOCATION        (unsigned char)8
#define HHDR_SUCCESS                    (unsigned char)9
/*=======================================================================*/
typedef unsigned char  HHDR_Return_Code;
typedef unsigned char  HHDR_UChar;
typedef unsigned char  HHDR_Bool;
typedef char*          HHDR_String;
typedef char           HHDR_Char;

typedef unsigned short HHDR_UShort;

typedef int            HHDR_Int;

typedef void           HHDR_None;
/*=======================================================================*/
typedef struct {
	const HHDR_String string;
	const HHDR_UChar type;
	const HHDR_UShort length;
} HHDR_Header;
/*=======================================================================*/
HHDR_Return_Code hhdr_parse(HHDR_Header header, Map* map);
HHDR_Return_Code hhdr_parse_head(HHDR_Header header, Map* map, HHDR_UShort* skiped);
HHDR_Return_Code hhdr_parse_body(HHDR_Header header, Map* map);
HHDR_Return_Code hhdr_head_collect_last(HHDR_String header, HHDR_String buffer);
HHDR_Return_Code hhdr_free_header(const HHDR_String header);

HHDR_Return_Code hhdr_is_sequence_valid(const HHDR_String _1, const HHDR_String _2, const HHDR_String _3, const HHDR_UChar type);
HHDR_Bool hhdr_is_method_valid(const HHDR_String method);
HHDR_Bool hhdr_is_allowed_chars_used(const HHDR_String string);
HHDR_Bool hhdr_is_http_version_valid(const HHDR_String http_version);
HHDR_Bool hhdr_is_status_code_valid(const HHDR_String status_code);

HHDR_String hhdr_create(Entries entries);
/*=======================================================================*/