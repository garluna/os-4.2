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

void* list_func(void* iter)
{
	int num_iter = (int) iter;
	int rand_index = 0;
	int i; 
	const char* keys[num_iter];
	SortedListElement_t* ret_elem;
	printf("Everything good before insert\n");
	// Insert (iteration) elements at random
	for(i = 0; i < num_iter; i++)
	{
		if(num_elements > 1)
			rand_index = rand() % (num_elements - 1); // 0 - (num_elements-1)

		printf("Inserting element at index %i \n", rand_index);
		SortedList_insert(head, elem_array[rand_index]);
		printf("After insert for iteration %i \n", i);
		keys[i] = elem_array[rand_index]->key; 
		printf("the key is %s \n", keys[i]);
	}
	
	// Find each inserted element, then delete it
	for(i = 0; i < num_iter; i++)
	{
		ret_elem = SortedList_lookup(head, keys[i]);
		if(ret_elem == NULL)
			fprintf(stderr, "ERROR: A thread failed to find an element it inserted \n");
		
		if(SortedList_delete(ret_elem) == 1) 
			fprintf(stderr, "ERROR: Corrupted pointers in delete attempt");	
	}

	pthread_exit(NULL);
}


void terminate()
{
	int i;
	free(head);
	for(i = 0; i < num_elements; i++)
	{
		if(elem_array[i] != NULL) 
			free(elem_array[i]);
	}
	free(elem_array);

}
