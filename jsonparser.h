/*
This header file defines the types and functions for parsing JSON files into objects
that can be queried. I recommend using functions provided by this file rather than accessing
the structs yourself in some cases
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mstr.h"

typedef enum { false, true } bool;
typedef enum { json_false, json_true, json_null} json_literal;
typedef enum { next_string_is_identifier, next_string_not_identifier, next_string_unknown } break_handler;

typedef enum
{
	kJsonObject,
	kJsonArray,
	kJsonNumber,
	kJsonString,
	kJsonLiteral
} json_type;

struct json_value
{
	//Refers to the type of the entity
	json_type type;
	
	//The potential values
	json_literal literal_value;
	char * string_value;
	double number_value;
	
	//This will only be allocated [by the default functions] if
	//the parent is an object
	char * key;
	
	//The parent will only be NULL if this is the uppermost entity
	//and if this element is in an array it will refer to the array
	//but if this element is in an object it will refer to the object
	struct json_value * parent;
	//If this is an array this will refer to the first element in the
	//array, however if this is an object it will refer to the first
	//property. This will always be NULL if the type is not an object or array
	struct json_value * first_child;
	//If this object is in an array this will refer to the next item
	//in the array, or the next property in the object. Can be NULL
	struct json_value * sibling;
};

typedef struct json_value JSON;

//This will parse a file with the given path, reading it using
//the standard C file i/o functions. If reading fails, the
//returned value will be NULL
JSON * json_value_from_file(char * filename);

//Create a new JSON value
JSON * json_value_new(json_type type);

//Will recursively free the children and siblings of a value
//so I recommended only calling this on the top level element
void json_value_free(JSON * value);

//Used to add a new key:value to an object or object to an array
void json_value_add_token(JSON ** parent, char * key, JSON * value);

//Query an object for a given key
JSON * json_value_for_key(JSON * obj, char * key);

//If the object is a string, the string value is returned. Otherwise the JSON string
//will be returned
char * json_value_to_string(JSON * obj);

//Determine if an object is a JSON literal and if so if the value is true
bool json_value_is_true(JSON * obj);

//Parsing functions

void json_entered_object(JSON ** v, char * lastIdentifier);
void json_exited_object(JSON ** v);
void json_entered_array(JSON ** v, char * lastIdentifier);
void json_exited_array(JSON ** v);
void json_entered_string(JSON ** v);
void json_found_character(JSON ** v, int character);
void json_found_string(JSON ** v, char * string, char ** lastIdentifier, bool stringIsIdentifier);
void json_exited_string(JSON ** v);
void json_entered_identifier(JSON ** v);
void json_entered_value(JSON ** v);
void json_found_literal(JSON ** v, json_literal literal, char * lastIdentifier);
void json_found_number(JSON ** v, double number, char * lastIndentifier);
break_handler json_handle_breaks(JSON ** v, int c, char * lastIdentifier);