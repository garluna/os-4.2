#include <stdlib.h>
#include <stdio.h>

#include "sltest.h"
#include "SortedList.h"

SortedList_t* initialize_list(SortedList_t* head)
{
	head = (SortedList_t*) malloc(sizeof(SortedList_t));
	if (head == NULL)
		return NULL; 

	head->prev = head;
	head->next = head;
	head->key = NULL;

	return head;
}

char* generate_key()
{
	int rand_key = rand() % 10000000 + 1; // Range: 1 to 10,0000,000

	char* s_key = malloc(9*sizeof(char));
	if(snprintf(s_key,9,"%i",rand_key) <= -1 || s_key == NULL)
		return NULL; 

	return s_key;
}

SortedListElement_t* initialize_element(SortedListElement_t* element)
{
	element = (SortedListElement_t*) malloc(sizeof(SortedListElement_t)); 
	if (element == NULL)
		return NULL;

	element->prev = NULL;
	element->next = NULL;
	element->key = generate_key(); 
	if(element->key == NULL)
		return NULL;

	return element;
}

void terminate()
{
	free(head);
}