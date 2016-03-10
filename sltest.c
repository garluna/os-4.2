#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "sltest.h"
#include "SortedList.h"



SortedList_t* initialize_list(SortedList_t* head_t)
{
	head_t = (SortedList_t*) malloc(sizeof(SortedList_t));
	if (head_t == NULL)
		return NULL; 

	head_t->prev = head_t;
	head_t->next = head_t;
	char* a = "head";
	head_t->key = a;

	return head_t;
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

void* list_func(void* index)
{
	int end_index = (int) index;
	int i;
	int elem_index = end_index - num_iterations;

	const char* keys[num_iterations];
	SortedListElement_t* ret_elem;
	// Insert (iteration) elements at random
	elem_index++;

	for(i = 0 ; elem_index <= end_index; elem_index++)
	{
	
		SortedList_insert(head, elem_array[elem_index]);
		keys[i] = elem_array[elem_index]->key; 
		i++;
	}
	
	
	// Find each inserted element, then delete it
	for(i = 0; i < num_iterations; i++)
	{
		ret_elem = SortedList_lookup(head, keys[i]);
	
	
	
		if(ret_elem == NULL)
			fprintf(stderr, "ERROR: A thread failed to find an element it inserted \n");
		
		if(SortedList_delete(ret_elem) == 1) 
			fprintf(stderr, "ERROR: Corrupted pointers in delete attempt \n");	
	}

	pthread_exit(NULL);
}


void terminate()
{
	int i;
	free(head);
	for(i = 0; i < num_elements; i++)
	{
		if(elem_array[i] != NULL) {
			free((void*)elem_array[i]->key);
			free(elem_array[i]);
		}
	}
	free(elem_array);

}
