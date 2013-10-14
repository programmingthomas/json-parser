#include "mstr.h"

mstr * mutable_string_create()
{
	mstr * str = malloc(sizeof(mstr));
	if (!str) return NULL;
	
	str->data = calloc(0, sizeof(char));
	if (!str->data)
	{
		free(str);
		return NULL;
	}
	
	str->length = str->capacity = 0;
	
	return str;
}

int mutable_string_expand(mstr * str, int expandBy)
{
	if (str->length + expandBy >= str->capacity)
	{
		int i;
		int newCapacity = MAX(str->capacity + expandBy, str->capacity * 2 + 1);
		char * new_str = realloc(str->data, newCapacity);
		if (!new_str) return -1;
		for (i = str->capacity; i < str->capacity + expandBy; i++) new_str[i] = 0;
		str->data = new_str;
		str->capacity = newCapacity;
		return str->capacity;
	}
	return 0;
}

void mutable_string_concat(mstr * str, char c)
{
	mutable_string_expand(str, 1);
	str->data[str->length] = c;
	str->length = str->length + 1;
}

void mutable_string_concat_string(mstr * str, char * string)
{
	int i = 0;
	int l = strlen(string);
	mutable_string_expand(str, l);
	//strcat?
	for (; i < l; i++) str->data[str->length + i] = string[i];
	str->length = str->length + l;
}

void mutable_string_free(mstr * str)
{
	if (str->data) free(str->data);
	free(str);
}

char * mutable_string_to_string(mstr * str)
{
	char * string = calloc(str->length + 1, 1);
	strcpy(string, str->data);
	string[str->length] = 0;
	return string;
}