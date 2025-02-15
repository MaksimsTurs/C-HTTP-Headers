#include "./Entries.h"

Ent_Return_Code entrie_push(Entries* entries, Ent_CString key, Ent_Any value, Ent_UShort value_size) {
	if(entries == NULL || key == NULL || (value != NULL && value_size == 0))
		return ENTRY_ERROR_INVALID_PARAM;

	if(entries->items == NULL) {
		entries->items = (Entrie**)malloc(sizeof(Entrie*));
	}	else {
		entries->items = (Entrie**)realloc(entries->items, (entries->size + 1) * sizeof(Entrie*));
		if(entries->items == NULL)
			return ENTRY_ERROR_MEMALLOC;
	}

	Ent_ULLong key_length = strlen(key) + 1;
	Ent_ULLong size_of_entrie = sizeof(Entrie);
	Entrie* item = (Entrie*)malloc(size_of_entrie + key_length + value_size);
	if(entries->items == NULL)
		return ENTRY_ERROR_MEMALLOC;

	Ent_String key_position = (Ent_String)item + size_of_entrie;
	Ent_Any value_position = key_position + key_length;

	item->key = key_position;
	mempcpy(item->key, key, key_length);

	if(value == NULL) {
		item->value = NULL;
	} else {
		item->value = value_position;
		mempcpy(item->value, value, value_size);
	}
	
	entries->items[entries->size++] = item;

	return ENTRY_SUCCES;
}

Ent_Return_Code entrie_pop(Entries entries) {
	if(entries.items == NULL)
		return ENTRY_ERROR_NULL_PTR;

	free(entries.items[entries.size - 1]);
	entries.items[entries.size - 1] = NULL;
	entries.size--;

	if(entries.size == 0)
		free(entries.items);

	return ENTRY_SUCCES;
}

Ent_Return_Code entrie_delete(Entries entries) {
	if(entries.items == NULL)
		return ENTRY_ERROR_NULL_PTR;

	for(Ent_ULLong index = 0; index < entries.size; index++)
		free(entries.items[index]);
	free(entries.items);

	return ENTRY_SUCCES;
}