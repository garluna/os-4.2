#include <stdlib.h>
#include <stdio.h>


#include "SortedList.h"
#include "parser.h"
#include "sltest.h"

/** GLOBALS **/ 
int num_threads; // Default 1 thread
int num_iterations; // Default 1 iteration
int num_lists;// Default 1 list 
int use_mutex; // Default 0 (false)
int use_spinlock; // Default 0 (false)
int insert_yield; // Default 0 (false)
int delete_yield; // Default 0 (false)
int search_yield; // Default 0 (false)
SortedList_t *head;
SortedListElement_t** elem_array;
int num_elements;

int main(int argc, char** argv)
{
	int returnStatus;
	int i;

	// Parse command line arguments
	returnStatus = parse(argc,argv);
	if (returnStatus != 0)
		return returnStatus;
	
	// Initialize empty list
	head = initialize_list(head);

	// Create elem_array
	num_elements = num_threads*num_iterations; 
	elem_array = malloc((num_elements)*sizeof(SortedListElement_t*));
	if(elem_array == NULL)
	{
		returnStatus = 1;
		fprintf(stderr,"ERROR: Unable to malloc the SortedListElement_t array");
		return returnStatus;
	}
	// Initialize elements in elem_array
	for(i = 0; i < num_elements; i++)
	{
		elem_array[i] = initialize_element(elem_array[i]);
		if(elem_array[i] == NULL)
		{
			fprintf(stderr, "ERROR: Unable to initialize element %i \n",i);
			returnStatus = 1;
			return returnStatus;
		}
	}
	
	int size = SortedList_length(head);
	printf("The size of the initialized list is %i \n", size);

	for(i = 0; i < num_elements; i++)
	{
		SortedList_insert(head,elem_array[i]);
	}
	/*
	SortedListElement_t* elem1 = initialize_element(elem1);
	SortedListElement_t* elem2 = initialize_element(elem2);
	SortedList_insert(head,elem1);
	SortedList_insert(head,elem2);
	*/
	size = SortedList_length(head);
	printf("The size of the initialized list is now %i \n", size);
	
	terminate();
	return returnStatus;
	return 0;
}