#include "./Entries.h"

Ent_Return_Code ent_push(Entries* entries, const Ent_String key, const Ent_Any value, const Ent_UChar value_size) {
	if(entries == NULL || key == NULL || ((value != NULL && value_size == 0) || (value == NULL && value_size > 0)))
		return ENT_ERROR_INVALID_PTR;

	Entrie** items = NULL;
	Entrie* item = NULL;
	Ent_ULLong key_length = 0;

	if(entries->items == NULL) {
		// Entrie array is empty, alloc memory for first element.
		items = (Entrie**)malloc(sizeof(Entrie*));
		if(items == NULL)
			return ENT_ERROR_MEMALLOC;
	}	else {
		// Realloc new memory for one new element (pointer on element).
		items = (Entrie**)realloc(entries->items, (entries->size + 1) * sizeof(Entrie*));
		if(items == NULL)
			return ENT_ERROR_MEMALLOC;
	}
	
	key_length = sb_string_length(key) + 1;
	if(key_length > ENT_KEY_MAX_LENGTH)
		return ENT_ERROR_INVALID_KEY_LENGTH;

	item = (Entrie*)malloc(sizeof(Entrie) + key_length + value_size);
	if(item == NULL)
		return ENT_ERROR_MEMALLOC;

	item->key = (Ent_String)item + sizeof(Entrie);
	memcpy(item->key, key, key_length);

	if(value == NULL) {
		// Save null value in to the entrie.
		item->value = NULL;
	} else {
		item->value = item->key + key_length;
		memcpy(item->value, value, value_size);
	}

	entries->items = items;
	entries->items[entries->size++] = item;

	return ENT_SUCCESS;
}

Ent_Return_Code ent_pop(Entries* entries) {
	if(entries == NULL || entries->items == NULL)
		return ENT_ERROR_INVALID_PTR;
	else if(entries->size <= 0)
		return ENT_ERROR_INVALID_SIZE;

	Entrie** items = NULL;

	entries->size--;

	// Realloc the memory with new size (without the last element).
	items = (Entrie**)realloc(entries->items, entries->size * sizeof(Entrie*));
	if(items == NULL)
		return ENT_ERROR_MEMALLOC;
	entries->items = items;

	// Remove free items when entrie array is empty.
	if(entries->size == 0) {
		free(entries->items);
		entries->items = NULL;
	}

	return ENT_SUCCESS;
}

Ent_Return_Code ent_free(Entries* entries) {
	if(entries == NULL || entries->items == NULL)
		return ENT_ERROR_INVALID_PTR;

	// Remove free items when entrie is empty.
	for(Ent_UInt index = 0; index < entries->size; index++) {
		free(entries->items[index]);
		entries->items[index] = NULL;
	}
		
	free(entries->items);
	entries->items = NULL;
	entries->size = 0;

	return ENT_SUCCESS;
}