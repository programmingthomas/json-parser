#include "jsonparser.h"

JSON * json_value_from_file(char * filename)
{
	JSON * v = NULL;
	
	bool inString = false, lastWasEscape = false;
	bool parsingNumber = false;
	bool stringIsIdentifier = false;
	
	mstr * parsedNumberString;
	
	char * lastIdentifier = NULL;
	
	mstr * parsingString;
	int parsingCodePoint = -1, codePoint = 0;
	FILE * fp;
	fp = fopen(filename, "r");
	int c;
	while ((c = fgetc(fp)) != EOF)
	{
		if (!inString && !parsingNumber)
		{
			break_handler h = json_handle_breaks(&v, c, lastIdentifier);
			if (h == next_string_is_identifier) stringIsIdentifier = true;
			else if (h == next_string_not_identifier) stringIsIdentifier = false;
			if (c == 'n') json_found_literal(&v, json_null, lastIdentifier);
			if (c == 't') json_found_literal(&v, json_true, lastIdentifier);
			if (c == 'f') json_found_literal(&v, json_false, lastIdentifier);
			if (c == '"')
			{
				json_entered_string(&v);
				inString = true;			
				parsingString = mutable_string_create();
				continue;
			}
			if ((c >= '0' && c <= '9') || c == '-')
			{
				parsingNumber = true;
				parsedNumberString = mutable_string_create();
			}
		}
		//Handle number parsing
		if (parsingNumber)
		{
			if ((c >= '0' && c <= '9') || c == '-' || c == '.' || c == 'E' || c == 'e' || c == '+')
			{
				mutable_string_concat(parsedNumberString, (char)c);
			}
			else
			{
				json_found_number(&v, strtod(parsedNumberString->data, NULL), lastIdentifier);
				parsingNumber = false;
				mutable_string_free(parsedNumberString);
				
				break_handler h = json_handle_breaks(&v, c, lastIdentifier);
				if (h == next_string_is_identifier) stringIsIdentifier = true;
				else if (h == next_string_not_identifier) stringIsIdentifier = false;
			}
		}
		
		if (inString)
		{
			int parsedChar = 0;
			if (parsingCodePoint >= 0)
			{
				int v = 0;
				if (c >= '0' && c <= '9') v = c - '0';
				else if (c >= 'a' && c <= 'f') v = c - 'a' + 10;
				else if (c >= 'A' && c <= 'F') v = c - 'A' + 10;
				codePoint = codePoint * 16 + v;
				parsingCodePoint++;
				if (parsingCodePoint == 4)
				{
					parsedChar = codePoint;
					parsingCodePoint = -1;
					codePoint = 0;
				}
			}
			else if (lastWasEscape)
			{
				if (c == '"') parsedChar = '\"';
				if (c == '\\') parsedChar ='\\';
				if (c == '/') parsedChar = '/';
				if (c == 'b') parsedChar = '\b';
				if (c == 'f') parsedChar = '\f';
				if (c == 'n') parsedChar = '\n';
				if (c == 'r') parsedChar = '\r';
				if (c == 't') parsedChar = '\t';
				if (c == 'u') parsingCodePoint = 0;
				lastWasEscape = false;
			}
			else
			{
				if (c == '\\') lastWasEscape = true;
				else if (c == '"')
				{
					inString = false;
					json_found_string(&v, parsingString->data, &lastIdentifier, stringIsIdentifier);
					free(parsingString);
					json_exited_string(&v);
				}
				else parsedChar = c;
			}
			if (parsedChar < 256 && parsedChar > 0)
			{
				mutable_string_concat(parsingString, (char)parsedChar);
			}
		}
	}
	fclose(fp);
	
	return v;
}

break_handler json_handle_breaks(JSON ** v, int c, char * lastIdentifier)
{
	printf("JSON handle breaks %c\n", c);
	if (c == '{')
	{
		json_entered_object(v, lastIdentifier);
		printf("JSON handle breaks: true\n");
		return next_string_is_identifier;
	}
	if (c == '}') json_exited_object(v);
	if (c == '[') json_entered_array(v, lastIdentifier);
	if (c == ']') json_exited_array(v);
	if (c == ':')
	{
		json_entered_value(v);
		return next_string_not_identifier;
	}
	if (c == ',')
	{
		printf("JSON handle breaks: true\n");
		return next_string_is_identifier;
	}
	printf("JSON handle breaks: false\n");
	return next_string_unknown;
}

void json_entered_object(JSON ** v, char * lastIdentifier)
{
	printf("JSON entered object\n");
	JSON * new_object = json_value_new(kJsonObject);
	json_value_add_token(v, lastIdentifier, new_object);
}

void json_exited_object(JSON ** v)
{
	if ((*v)->parent)
	{
		printf("JSON exited object\n");
		*v = (*v)->parent;
	}
}

void json_entered_array(JSON ** v, char * lastIdentifier)
{
	printf("JSON entered array\n");
	json_value_add_token(v, lastIdentifier, json_value_new(kJsonArray));
}

void json_exited_array(JSON ** v)
{
	if ((*v)->parent) *v = (*v)->parent;
}

void json_entered_identifier(JSON ** v) {}

void json_entered_value(JSON ** v) {}

void json_found_string(JSON ** v, char * string, char ** lastIdentifier, bool stringIsIdentifier)
{
	printf("JSON found string %s\n", string);
	if (stringIsIdentifier)
	{
		printf("String is identifier\n");
		*lastIdentifier = calloc(strlen(string), 1);
		strcpy(*lastIdentifier, string);
	}
	else
	{
		printf("String is value\n");
		JSON * obj_string = json_value_new(kJsonString);
		obj_string->string_value = string;
		json_value_add_token(v, *lastIdentifier, obj_string);
	}
}

void json_found_number(JSON ** v, double number, char * lastIdentifier)
{
	// printf("Found number %f for %s\n", number, lastIdentifier);
	JSON * obj = json_value_new(kJsonNumber);
	obj->number_value = number;
	json_value_add_token(v, lastIdentifier, obj);
}
void json_found_literal(JSON ** v, json_literal literal, char * lastIdentifier)
{
	JSON * obj = json_value_new(kJsonLiteral);
	obj->literal_value = literal;
	json_value_add_token(v, lastIdentifier, obj);
}

void json_entered_string(JSON ** v) {}
void json_exited_string(JSON ** v) {}
void json_found_character(JSON ** v, int character) {}

JSON * json_value_new(json_type type)
{
	JSON * obj = malloc(sizeof(JSON));
	obj->type = type;
	return obj;
}

void json_value_free(JSON * value)
{
	if (value != NULL)
	{
		if (value->type == kJsonString && value->string_value)
		{
			free(value->string_value);
		}
		if (value->key)
		{
			free(value->key);
		}

		if (value->first_child != NULL && (value->type == kJsonArray ||  value->type == kJsonObject))
		{
			json_value_free(value->first_child);
		}
		if (value->sibling != NULL)
		{
			json_value_free(value->sibling);
		}
		free(value);
		value = NULL;
	}
}

void json_value_add_token(JSON ** parent, char * key, JSON * obj)
{
	if (key)
	{
		obj->key = calloc(strlen(key), 1);
		strcpy(obj->key, key);
		printf("Assigned key %s\n", key);
	}
	//NEED TO REPLACE ALL v WITH POINTER TO PARENT!!!!!!
	if (!(*parent))
	{
		printf("Parent was null, setting parent\n");
		*parent = obj;
	}
	else if ((*parent)->type == kJsonObject || (*parent)->type == kJsonArray)
	{
		if (!(*parent)->first_child)
		{
			(*parent)->first_child = obj;
			obj->parent = *parent;
		}
		else
		{
			(*parent)->sibling = obj;
			obj->parent = (*parent)->parent;
		} 
		*parent = obj;
	}
	else
	{
		(*parent)->sibling = obj;
		obj->parent = (*parent)->parent;
		*parent = obj;
	}
}

JSON * json_value_for_key(JSON * obj, char * key)
{
	if (obj->type != kJsonObject)
	{
		//printf("%p is not a JSON object\n", obj);
		return NULL;
	} 
	else
	{
		JSON * item = obj->first_child;
		while (item)
		{
			if (strcmp(item->key, key) == 0) return item;
			item = item->sibling;
		}
		return NULL;
	}
}

char * json_value_to_string(JSON * obj)
{
	if (obj->type == kJsonString)
	{
		char * string = calloc(2 + strlen(obj->string_value), 1);
		sprintf(string, "%s", obj->string_value);
		return string;
	}
	else if (obj->type == kJsonLiteral)
	{
		switch (obj->literal_value)
		{
			case json_false:
				return "false";
				break;
			case json_true:
				return "true"; 
				break;
			case json_null:
				return "null";
				break;
			default:
				return "null";
				break;
		}
	}
	else if (obj->type == kJsonNumber)
	{
		char * number_string = calloc(20,1);
		snprintf(number_string, 20, "%f", obj->number_value);
		return number_string;
	}
	else if (obj->type == kJsonObject)
	{
		mstr * mutableStr = mutable_string_create();
		mutable_string_concat(mutableStr, '{');
		if (obj->first_child)
		{
			JSON * property = obj->first_child;
			while (property)
			{
				char * str_value = json_value_to_string(property);
				char * key = property->key;
				mutable_string_concat(mutableStr, '\"');
				mutable_string_concat_string(mutableStr, key);
				mutable_string_concat_string(mutableStr, "\":");
				mutable_string_concat_string(mutableStr, str_value);
				if (property->type != kJsonLiteral) free(str_value);
				property = property->sibling;
				if (property) mutable_string_concat(mutableStr, ',');
			}
		}
		mutable_string_concat(mutableStr, '}');
		char * data = mutable_string_to_string(mutableStr);
		mutable_string_free(mutableStr);
		return data;
	}
	else if (obj->type == kJsonArray)
	{
		mstr * mutableStr = mutable_string_create();
		mutable_string_concat(mutableStr, '[');
		if (obj->first_child)
		{
			JSON * property = obj->first_child;
			while (property)
			{
				char * str_value = json_value_to_string(property);
				mutable_string_concat_string(mutableStr, str_value);
				property = property->sibling;
				if (property) mutable_string_concat(mutableStr, ',');
			}
			mutable_string_concat(mutableStr, ']');
			char * data = mutable_string_to_string(mutableStr);
			mutable_string_free(mutableStr);
			return data;
		}
	}
	return NULL;
}

bool json_value_is_true(JSON * obj)
{
	if (obj->type == kJsonLiteral) return obj->literal_value == json_true;
	return false;
}