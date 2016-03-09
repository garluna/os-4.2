#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


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
	int i; // iterator variable
	struct timespec start;
	struct timespec end;
	long long startTime; 
	long long endTime;
	long long totalTime;


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

	// BEGIN CLOCK TIME
	if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
	{
		fprintf(stderr, "ERROR: clock_gettime start time\n");
		returnStatus = 1;
		return returnStatus;
	}

	/* DO THE THREAD STUFF */

	if (clock_gettime(CLOCK_MONOTONIC, &end) != 0)
	{
		fprintf(stderr, "ERROR: clock_gettime end time\n");
		exit(1);
	}

	startTime = (long long)(start.tv_sec*pow(10, 9) + start.tv_nsec);
	endTime = (long long)(end.tv_sec*pow(10, 9) + end.tv_nsec);
	totalTime = endTime - startTime;

	printf("elapsed time: %lld ns\n", totalTime);


	
	/*
	int size = SortedList_length(head); // SANITY CHECK
	printf("The size of the initialized list is %i \n", size);

	for(i = 0; i < num_elements; i++)
	{
		SortedList_insert(head,elem_array[i]);
	}

	size = SortedList_length(head);
	printf("The size of the initialized list is now %i \n", size); // SANITY CHECK
	*/

	terminate();
	return returnStatus;
	return 0;
}