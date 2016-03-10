#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "sltest.h"
#include "SortedList.h"


SortedList_t* initialize_list(SortedList_t* head_t)
{
	head_t = (SortedList_t*) malloc(sizeof(SortedList_t));
	if (head_t == NULL)
		return NULL; 

	head_t->prev = head_t;
	head_t->next = head_t;
	// Change back to null 
	char* a = "head";
	head_t->key = a;

	return head_t;
}

char* generate_key()
{
	int rand_key = rand() % 10000000 + 1; // Range: 1 to 10,0000,000

	char* s_key = malloc(10*sizeof(char));
	if(snprintf(s_key,10,"%i",rand_key) <= -1 || s_key == NULL)
		return NULL; 

	return s_key;
}

int hash_key(const char* key)
{
	int hash = 0;
    int c;

    while (c = *key++)
         hash = (hash + (hash >> 5)) + *(key++);

    return hash % num_lists;

}

void initialize_element(SortedListElement_t* element)
{
	//element = (SortedListElement_t*) malloc(sizeof(SortedListElement_t)); 
	if (element == NULL)
		exit(1);

	element->prev = NULL;
	element->next = NULL;
	element->key = generate_key(); 
	if(element->key == NULL)
		exit(1);	
}

void acquire_mutex(sublist_t* list)
{
	pthread_mutex_lock(&mutexes[list->lock_index]);
}

void release_mutex(sublist_t* list)
{
	pthread_mutex_unlock(&mutexes[list->lock_index]);
}

void acquire_lock(sublist_t* list)
{
	while(__sync_lock_test_and_set(&locks[list->lock_index], 1))
	{
		continue;
	}
}

void release_lock(sublist_t *list)
{
	__sync_lock_release(&locks[list->lock_index]);
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
	
		SortedList_insert(head, &elem_array[elem_index]);
		keys[i] = elem_array[elem_index].key; 
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

void* sublist_func(void* index)
{
	int end_index = (int) index;
	int i;
	int elem_index = end_index - num_iterations;
	int sublist_index; 

	SortedListElement_t* ret_elem;
	// Insert (iteration) elements at random
	elem_index++;

	for( ; elem_index <= end_index; elem_index++)
	{
		sublist_index = hash_key(elem_array[elem_index].key);
		
		if(use_mutex && !use_spinlock)
			acquire_mutex(&sublists[sublist_index]);
		if(!use_mutex && use_spinlock)
			acquire_lock(&sublists[sublist_index]);

		SortedList_insert(sublists[sublist_index].sub_head, &elem_array[elem_index]);

		if(use_mutex && !use_spinlock)
			release_mutex(&sublists[sublist_index]);
		if(!use_mutex && use_spinlock)
			release_lock(&sublists[sublist_index]);
	}
	
	elem_index = end_index - num_iterations;
	elem_index++;

	for( ; elem_index <= end_index; elem_index++)
	{
		sublist_index = hash_key(elem_array[elem_index].key);
		ret_elem = SortedList_lookup(sublists[sublist_index].sub_head, elem_array[elem_index].key);
		
		if(ret_elem == NULL)
			fprintf(stderr, "ERROR: A thread failed to find an element it inserted \n");
		
		if(use_mutex && !use_spinlock)
			acquire_mutex(&sublists[sublist_index]);
		if(!use_mutex && use_spinlock)
			acquire_lock(&sublists[sublist_index]);

		if(SortedList_delete(ret_elem) == 1) 
			fprintf(stderr, "ERROR: Corrupted pointers in delete attempt \n");	

		if(use_mutex && !use_spinlock)
			release_mutex(&sublists[sublist_index]);
		if(!use_mutex && use_spinlock)
			release_lock(&sublists[sublist_index]);
	}
	
	pthread_exit(NULL);
}

int SortedList_length_of_all_sublists()
{
	int i; 
	int total = 0;
	for(i = 0; i < num_lists; i++)
	{
		total += SortedList_length(sublists[i].sub_head);	
	}
	return total; 
}


void terminate()
{
	int i;
	free(head);
	for(i = 0; i < num_elements; i++)
	{
		free((void*)elem_array[i].key);
	}
	free(elem_array);

	if(num_lists > 1)
	{
		
	}

}
