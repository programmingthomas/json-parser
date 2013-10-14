#include <stdio.h>
#include <string.h>
#include "jsonparser.h"

//void reddit(char ** argv);

int main(int argc, char ** argv)
{
	JSON * value = json_value_from_file("tests.json");
	printf("Title: %s\n", json_value_for_key(value, "title")->string_value);
	printf("Description: %s\n\n", json_value_for_key(value, "description")->string_value);
	
	JSON * historyItem = json_value_for_key(value, "history")->first_child;
	while (historyItem)
	{
		printf("Year: %d\n", (int)json_value_for_key(historyItem, "year")->number_value);
		printf("Title: %s\n", json_value_for_key(historyItem, "title")->string_value);
		printf("Description: %s\n\n", json_value_for_key(historyItem, "description")->string_value);
		historyItem = historyItem->sibling;
	}
	
	if (json_value_is_true(json_value_for_key(value, "is_awesome")))
		printf("JSON is awesome\n");
	else printf("JSON is not awesome\n");
	
	json_value_free(value);
	return 0;
}

/*void reddit(char ** argv)
{
	JSON * value = json_value_from_file(argv[1]);
	printf("Parsed\n");
	JSON * kind = json_value_for_key(value, "kind");
	printf("kind = %s\n", kind->string_value);
	if (strcmp(kind->string_value, "t2") == 0)
	{
		printf("This is a user\n");
		JSON * data = json_value_for_key(value, "data");
		if (json_value_is_true(json_value_for_key(data, "has_mail")))
			printf("User has mail\n");
		else printf("User doesn't have mail\n");
		printf("Username is %s\n", json_value_for_key(data, "name")->string_value);
		printf("Created %f\n", json_value_for_key(data, "created")->number_value);
		printf("Modhash: %s\n", json_value_for_key(data, "modhash")->string_value);
			int link_karma = (int)json_value_for_key(data, "link_karma")->number_value;
		int comment_karma = (int)json_value_for_key(data, "comment_karma")->number_value;
		printf("Link karma: %d\tComment karma:%d\tTotal karma:%d\n", link_karma, comment_karma, link_karma + comment_karma);
		if (json_value_is_true(json_value_for_key(data, "over_18")))
			printf("Is an adult\n");
		else printf("Is a child\n");
		if (json_value_is_true(json_value_for_key(data, "is_gold")))
			printf("Has gold\n");
		else printf("Doesn't have gold\n");
		if (json_value_is_true(json_value_for_key(data, "is_mod")))
			printf("Is mod\n");
		else printf("Isn't mod\n");
		if (json_value_is_true(json_value_for_key(data, "has_verified_email")))
			printf("Has verified email\n");
		else printf("Hasn't verified email\n");
		printf("ID: %s\n", json_value_for_key(data, "id")->string_value);
		if (json_value_is_true(json_value_for_key(data, "has_mod_mail")))
			printf("Has moderator mail\n");
		else printf("Doesn't have mod mail\n");
	}
	else if (strcmp(kind->string_value, "Listing")  == 0)
	{
		printf("This is a list\n");
		JSON * data = json_value_for_key(value, "data");
		JSON * children = json_value_for_key(data, "children");
		JSON * child = children->first_child;
		while (child)
		{
			JSON * cdata = json_value_for_key(child, "data");
			char * subreddit = json_value_for_key(cdata, "subreddit")->string_value;
			char * author = json_value_for_key(cdata, "author")->string_value;
			printf("%s in %s\n", author, subreddit);
			child = child->sibling;
		}
	}
	//Frees all children
	json_value_free(value);
}*/