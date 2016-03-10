#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>


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
pthread_t* threads;

int main(int argc, char** argv)
{
	int returnStatus;
	int i; // iterator variable
	int size;
	int end_index; // Tracks the max index passed to each thread

	struct timespec start;
	struct timespec end;
	long long startTime; 
	long long endTime;
	long long totalTime;


	// Parse command line arguments
	returnStatus = parse(argc,argv);
	if (returnStatus != 0)
		return returnStatus;
	
	if(num_lists == 1)
	{
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

		// Create threads 
		threads = malloc(num_threads*sizeof(pthread_t));
		if(threads == NULL)
		{
			returnStatus = 1;
			fprintf(stderr,"ERROR: Unable to malloc the threads array");
			return returnStatus;
		}

		end_index = -1;
		for(i = 0; i < num_threads; i++)
		{
			end_index += num_iterations;
			if (pthread_create(&threads[i], NULL, (void*) list_func, (void*)end_index) != 0)
			{
				returnStatus = 1;
				fprintf(stderr, "ERROR: Unable to create a thread \n");
				return returnStatus;
			}
		}

		// Wait for all threads to complete
		for(i = 0; i < num_threads; i++)
		{
			if (pthread_join(threads[i], NULL) != 0)
			{
				returnStatus = 1;
				fprintf(stderr, "ERROR: Unable to join a thread \n");
				return returnStatus;	
			}
		}
		if (clock_gettime(CLOCK_MONOTONIC, &end) != 0)
		{
			fprintf(stderr, "ERROR: clock_gettime end time\n");
			exit(1);
		}

		startTime = (long long)(start.tv_sec*pow(10, 9) + start.tv_nsec);
		endTime = (long long)(end.tv_sec*pow(10, 9) + end.tv_nsec);
		totalTime = endTime - startTime;
		int total_operations = num_iterations*num_threads*(num_iterations*3)*4;


		// Print Output:
		size = SortedList_length(head);
		if(size != 0)
			fprintf(stderr, "ERROR:The size of list is %i \n", size);
		
		printf("%d threads x %d iterations x (ins + lookup/del) x (100/2) = %d operations\n", num_threads, num_iterations, total_operations);
		printf("elapsed time: %lld ns\n", totalTime);
		printf("per operation: %i ns \n", totalTime/total_operations);
	}

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
}
