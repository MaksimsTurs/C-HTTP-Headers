#include "./HTTPHeader.h"

HHDR_Return_Code hhdr_parse(HHDR_Header header, Map* map) {
	HHDR_UShort skiped_chars = 0;

	HHDR_Return_Code map_init_code = map_init(map, HHDR_DEFAULT_MAP_SIZE);
	if(map_init_code != MAP_SUCCESS)
		return map_init_code;

	HHDR_Return_Code hhdr_head_parsing_code = hhdr_parse_head(header, map, &skiped_chars);
	if(hhdr_head_parsing_code != HHDR_SUCCESS)
		return hhdr_head_parsing_code;

	// HHDR_Return_Code hhdr_body_parsing_code = hhdr_parse_body(raw, map);
	// if(hhdr_body_parsing_code != HHDR_SUCCESS)
	// 	return hhdr_body_parsing_code;

	return HHDR_SUCCESS;
}

HHDR_Return_Code hhdr_parse_head(HHDR_Header header, Map* map, HHDR_UShort* skiped) {
	HHDR_String f_space = NULL;
	HHDR_String s_space = NULL;
	HHDR_Return_Code is_valid = 0;
	HHDR_Char http_version[10] = {0};
		
	if(hhdr_is_allowed_chars_used(header.string) != HHDR_TRUE)
		return HHDR_ERROR_UNALLOWED_CHARACTER;

	f_space = strchr(header.string, HHDR_WHITE_SPACE);
	s_space = strchr(f_space + 1, HHDR_WHITE_SPACE);	

	switch(header.type) {
		case HHDR_HEADER_RESPONSE:
			// Response header HTTP/1.1 | 500 | Internal Server Error
			HHDR_Char status_code[5] = {0};
			HHDR_Char status_text[35] = {0};
		
			// TODO: f_spec - header.string, buffer overflow check
			memcpy(http_version, header.string, f_space - header.string);
			http_version[f_space - header.string] = '\0';
			// TODO: f_spec - header.string, buffer overflow check
			memcpy(status_code, f_space + 1, s_space - f_space - 1);
			status_code[s_space - f_space] = '\0';
			
			hhdr_head_collect_last(s_space + 1, status_text);
			// TODO: f_spec - header.string, buffer overflow check
			HHDR_UChar status_text_length = strlen(status_text);
			status_text[status_text_length - 1] = '\0';

			is_valid = hhdr_is_sequence_valid(http_version, status_code, status_text, header.type);
			if(is_valid != HHDR_SUCCESS)
				return is_valid;
			
			map_set_item(map, "HTTP-Version\0", http_version, strlen(http_version) + 1);
			map_set_item(map, "Status-Code\0", status_code, strlen(status_code) + 1);
			map_set_item(map, "Status-Text\0", status_text, status_text_length);
		break;
		case HHDR_HEADER_REQUEST:
			// Request  header GET /index.html HTTP/1.1
			HHDR_Char method[3] = {0};
			HHDR_Char path[550] = {0};
			
			// TODO: f_spec - header.string, buffer overflow check
			memcpy(method, header.string, f_space - header.string);
			method[f_space - header.string] = '\0';
			
			// TODO: f_spec - header.string, buffer overflow check
			memcpy(path, f_space + 1, s_space - f_space - 1);
			path[s_space - f_space] = '\0';
			
			hhdr_head_collect_last(s_space + 1, http_version);
			// TODO: f_spec - header.string, buffer overflow check
			HHDR_UChar http_version_length = strlen(http_version);
			http_version[http_version_length - 1] = '\0';
			
			is_valid = hhdr_is_sequence_valid(method, path, http_version, header.type);
			if(is_valid != HHDR_SUCCESS)
				return is_valid;
			
			map_set_item(map, "Method\0", method, strlen(method) + 1);
			map_set_item(map, "Path\0", path, strlen(path) + 1);
			map_set_item(map, "HTTP-Version\0", http_version, http_version_length);

			for(int index = 0; index < map->size; index++) {
				if(map->items[index]) printf("%s %s\n", map->items[index]->key, map->items[index]->value);
			}
		break;
		default:
			return HHDR_ERROR_ILLEGAL_HEAD_TYPE;
		break;
	}

	return HHDR_SUCCESS;
}

HHDR_Return_Code hhdr_head_collect_last(HHDR_String header, HHDR_String buffer) {
	HHDR_Char ch = ' ';
	HHDR_UShort last_size = 0;

	while(ch != HHDR_CARRIAGE_RETURN) {
		ch = *header;
		header++;
		last_size++;
	}

	// TODO: f_spec - header.string, buffer overflow check
	memcpy(buffer, header - last_size, last_size);

	return HHDR_SUCCESS;
}

HHDR_Return_Code hhdr_is_sequence_valid(const HHDR_String _1, const HHDR_String _2, const HHDR_String _3, const HHDR_UChar type) {
	switch(type) {
		case HHDR_HEADER_RESPONSE:
			if(hhdr_is_status_code_valid(_2) != HHDR_TRUE)
				return HHDR_ERROR_ILLEGAL_STATUS_CODE;
			else if(hhdr_is_http_version_valid(_1) != HHDR_TRUE)
				return HHDR_ERROR_ILLEGAL_HTTP_VERSION;
			else 
				return HHDR_SUCCESS;
		break;
		case HHDR_HEADER_REQUEST:
			if(hhdr_is_method_valid(_1) != HHDR_TRUE)
				return HHDR_ERROR_ILLEGAL_METHOD;
			else if(hhdr_is_http_version_valid(_3) != HHDR_TRUE)
				return HHDR_ERROR_ILLEGAL_HTTP_VERSION;
			else
				return HHDR_SUCCESS;
		break;
		default:
			return HHDR_ERROR_ILLEGAL_HEAD_TYPE;
		break;
	}
}

HHDR_Bool hhdr_is_status_code_valid(const HHDR_String status_code) {
	HHDR_Int _status_code = atoi(status_code);

	if(_status_code < HHDR_HTTP_MIN_CODE || _status_code > HHDR_HTTP_MAX_CODE)
		return HHDR_FALSE;

	return HHDR_TRUE;
}

HHDR_Bool hhdr_is_method_valid(const HHDR_String method) {
	HHDR_String methods[5] = HHDR_HTTP_METHODS;
	HHDR_Bool is_exist = HHDR_FALSE;

	for(HHDR_UChar index = 0; index < 5; index++) {
		if(strcmp(methods[index], method) == 0) {
			is_exist = HHDR_TRUE;
			break;
		}
	}
 
	return is_exist;
}

HHDR_Bool hhdr_is_http_version_valid(const HHDR_String http_version) {
	HHDR_String http_versions[3] = HHDR_HTTP_VERSIONS;
	HHDR_Bool is_valid = HHDR_FALSE;

	for(HHDR_UChar index = 0; index < 3; index++) {
		if(strcmp(http_versions[index], http_version) == 0) {
			is_valid = HHDR_TRUE;
			break;
		}
	}

	return is_valid;
}

HHDR_Bool hhdr_is_allowed_chars_used(const HHDR_String string) {
	HHDR_Bool is_valid = HHDR_TRUE;
	HHDR_Char ch = ' ';
	HHDR_String str_cmp = string;

	while(ch != HHDR_CARRIAGE_RETURN) {
		ch = *str_cmp;
		
		if((ch < 32 || ch > 126) && ch != 13) {
			is_valid = HHDR_FALSE;
			break;
		}

		str_cmp++;
	}

	return is_valid;
}

// HHDR_Return_Code hhdr_parse_body(HHDR_Raw raw, Map* map) {
// 	HHDR_Char ch = ' ';
// 	HHDR_UShort key_length = 0;
// 	HHDR_UShort value_size = 0;
// 	HHDR_Bool is_key = HHDR_TRUE;

// 	while(ch != '\0') {
// 		ch = *raw.string;

// 		if(ch == HHDR_NEW_LINE) {
// 			if(key_length > raw.config->key_length)
// 				return HHDR_ERROR_ILLEGAL_KEY_LENGTH;
// 			else if(value_size > raw.config->value_size)
// 				return HHDR_ERROR_ILLEGAL_VALUE_SIZE;

// 			HHDR_String key = (HHDR_String)malloc(key_length);
// 			if(key == NULL)
// 				return HHDR_ERROR_MEMALLOCATION;
// 			HHDR_String value = (HHDR_String)malloc(value_size);
// 			if(value == NULL)
// 				return HHDR_ERROR_MEMALLOCATION;

// 			memcpy(key, raw.string - (key_length + value_size) - 3, key_length);
// 			key[key_length] = '\0';

// 			memcpy(value, raw.string - (value_size + 1), value_size);
// 			value[value_size] = '\0';

// 			map_set_item(map, key, value, value_size);

// 			free(key);
// 			free(value);

// 			key_length = 0;
// 			value_size = 0;
// 			is_key = HHDR_TRUE;
// 		} else {
// 			// Check if we on the end of key string position.
// 			if(ch == HHDR_BODY_SEPARATOR && is_key == HHDR_TRUE) {
// 				is_key = HHDR_FALSE;
// 				raw.string += 2;
// 				continue;
// 			}
// 			// Count bytes only for visible characters.
// 			if(is_key && ch != HHDR_CARRIAGE_RETURN && ch != HHDR_NEW_LINE)
// 				key_length++;
// 			else if(!is_key && ch != HHDR_CARRIAGE_RETURN && ch != HHDR_NEW_LINE)
// 				value_size++;
// 		}

// 		raw.string++;
// 	}

// 	return HHDR_SUCCESS;
// }

// HHDR_String hhdr_create(Entries entries) {
// 	HHDR_String buffer = (HHDR_String)calloc(1, 1);
// 	HHDR_UChar key_length = 0;
// 	HHDR_UShort value_size = 0;
// 	HHDR_UShort header_size = 0;
	
// 	for(Ent_UInt index = 0; index < entries.size; index++) {
// 		key_length = strlen(entries.items[index]->key);
// 		value_size = strlen(entries.items[index]->value);

// 		buffer = realloc(buffer, header_size + key_length + value_size + 5);

// 		memcpy(buffer + header_size, entries.items[index]->key, key_length);		
// 		memcpy(buffer + (header_size + key_length), ": ", 2);
// 		memcpy(buffer + (header_size + key_length + 2), entries.items[index]->value, value_size);
// 		memcpy(buffer + (header_size + key_length + value_size + 2), "\r\n", 3);

// 		header_size += key_length + value_size + 4;
// 	}

// 	buffer[header_size] = '\0';

// 	return buffer;
// }

// HHDR_Return_Code hhdr_delete_header(HHDR_CString header) {
// 	if(header == NULL)
// 		free(header);
// }