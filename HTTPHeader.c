#include "./HTTPHeader.h"

HHDR_Return_Code hhdr_parse(HHDR_Raw raw, Map* map) {
	HHDR_UShort skiped_chars = 0;

	if(raw.config == NULL) {
		raw.config->key_length = HHDR_DEFAULT_KEY_LENGTH;
		raw.config->value_size = HHDR_DEFAULT_VALUE_SIZE;
		raw.config->map_size = HHDR_DEFAULT_MAP_INIT_SIZE;
	} else {
		raw.config->key_length = raw.config->key_length == 0 ? HHDR_DEFAULT_KEY_LENGTH : raw.config->key_length;
		raw.config->value_size = raw.config->value_size == 0 ? HHDR_DEFAULT_VALUE_SIZE : raw.config->value_size;
		raw.config->map_size   = raw.config->map_size == 0 ? HHDR_DEFAULT_MAP_INIT_SIZE : raw.config->map_size;
	}

	HHDR_Return_Code map_init_code = map_init(map, raw.config->map_size);
	if(map_init_code != MAP_SUCCESS)
		return map_init_code;

	HHDR_Return_Code hhdr_head_parsing_code = hhdr_parse_head(raw, map, &skiped_chars);
	if(hhdr_head_parsing_code != HHDR_SUCCES)
		return hhdr_head_parsing_code;

	raw.string += skiped_chars + 1;

	HHDR_Return_Code hhdr_body_parsing_code = hhdr_parse_body(raw, map);
	if(hhdr_body_parsing_code != HHDR_SUCCES)
		return hhdr_body_parsing_code;

	return HHDR_SUCCES;
}

HHDR_Return_Code hhdr_parse_head(HHDR_Raw raw, Map* map, HHDR_UShort* skiped) {
	HHDR_Char ch = ' ';
	HHDR_CString keys[HHDR_HEADER_HEAD_KEYS_LENGTH] = { "Method\0", "Path\0", "HTTP-Version\0" };
	HHDR_UChar value_size = 0;
	HHDR_UChar keys_index = 0;
	HHDR_UShort local_skiped = 0;

	while(ch != HHDR_CARRIAGE_RETURN) {
		ch = *raw.string;
		if(ch != HHDR_WHITE_SPACE && ch != HHDR_CARRIAGE_RETURN && ch != HHDR_NEW_LINE) {
			value_size++;
		} else {
			// Validate value size.
			if(value_size > raw.config->value_size)
				return HHDR_ERROR_ILLEGAL_VALUE_SIZE;
		
			HHDR_String value = (HHDR_String)malloc(value_size);
			if(value == NULL)
				return HHDR_ERROR_MEMALLOCATION;

			memcpy(value, raw.string - value_size, value_size);
			value[value_size] = '\0';

			map_set_item(map, keys[keys_index], value, value_size);

			keys_index++;
			memset(value, 0, value_size);
			value_size = 0;
			free(value);
		}

		local_skiped++;
		raw.string++;
	}

	*skiped = local_skiped;

	return HHDR_SUCCES;
}

HHDR_Return_Code hhdr_parse_body(HHDR_Raw raw, Map* map) {
	HHDR_Char ch = ' ';
	HHDR_UShort key_length = 0;
	HHDR_UShort value_size = 0;
	HHDR_Bool is_key = HHDR_TRUE;

	while(ch != '\0') {
		ch = *raw.string;

		if(ch == HHDR_NEW_LINE) {
			if(key_length > raw.config->key_length)
				return HHDR_ERROR_ILLEGAL_KEY_LENGTH;
			else if(value_size > raw.config->value_size)
				return HHDR_ERROR_ILLEGAL_VALUE_SIZE;

			HHDR_String key = (HHDR_String)malloc(key_length);
			if(key == NULL)
				return HHDR_ERROR_MEMALLOCATION;
			HHDR_String value = (HHDR_String)malloc(value_size);
			if(value == NULL)
				return HHDR_ERROR_MEMALLOCATION;

			memcpy(key, raw.string - (key_length + value_size) - 3, key_length);
			key[key_length] = '\0';

			memcpy(value, raw.string - (value_size + 1), value_size);
			value[value_size] = '\0';

			map_set_item(map, key, value, value_size);

			free(key);
			free(value);

			key_length = 0;
			value_size = 0;
			is_key = HHDR_TRUE;
		} else {
			// Check if we on the end of key string position.
			if(ch == HHDR_BODY_SEPARATOR && is_key == HHDR_TRUE) {
				is_key = HHDR_FALSE;
				raw.string += 2;
				continue;
			}
			// Count bytes only for visible characters.
			if(is_key && ch != HHDR_CARRIAGE_RETURN && ch != HHDR_NEW_LINE)
				key_length++;
			else if(!is_key && ch != HHDR_CARRIAGE_RETURN && ch != HHDR_NEW_LINE)
				value_size++;
		}

		raw.string++;
	}

	return HHDR_SUCCES;
}

HHDR_String hhdr_create(Entries entries) {
	HHDR_String buffer = (HHDR_String)calloc(1, 1);
	HHDR_UChar key_length = 0;
	HHDR_UShort value_size = 0;
	HHDR_UShort header_size = 0;
	
	for(Ent_UInt index = 0; index < entries.size; index++) {
		key_length = strlen(entries.items[index]->key);
		value_size = strlen(entries.items[index]->value);

		buffer = realloc(buffer, header_size + key_length + value_size + 5);

		memcpy(buffer + header_size, entries.items[index]->key, key_length);		
		memcpy(buffer + (header_size + key_length), ": ", 2);
		memcpy(buffer + (header_size + key_length + 2), entries.items[index]->value, value_size);
		memcpy(buffer + (header_size + key_length + value_size + 2), "\r\n", 3);

		header_size += key_length + value_size + 4;
	}

	buffer[header_size] = '\0';

	return buffer;
}

HHDR_Return_Code hhdr_delete_header(HHDR_CString header) {
	if(header == NULL)
		free(header);
}