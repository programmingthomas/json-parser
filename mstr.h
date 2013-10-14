#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct
{
	char * data;
	int length;
	int capacity;
} mstr;

mstr * mutable_string_create();
void mutable_string_concat(mstr * str, char c);
void mutable_string_concat_string(mstr * str, char * string);
void mutable_string_free(mstr * str);
int mutable_string_expand(mstr * str, int expandBy);
//This DOES NOT return a pointer to the string
//it instead will create a new string of the str->length
//rather than strlen(str->data)
char * mutable_string_to_string(mstr * str);