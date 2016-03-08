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


int main(int argc, char** argv)
{
	int returnStatus = parse(argc,argv);
	
	head = initialize_list(head);
	int size = SortedList_length(head);
	printf("The size of the initialized list is %i \n", size);

	SortedListElement_t* elem1 = initialize_element(elem1);
	SortedListElement_t* elem2 = initialize_element(elem2);
	SortedList_insert(head,elem1);
	SortedList_insert(head,elem2);
	size = SortedList_length(head);
	printf("The size of the initialized list is %i \n", size);

	terminate();
	return returnStatus;
}