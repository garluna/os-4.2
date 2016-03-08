#include <stdlib.h>
#include <stdio.h>

#include "SortedList.h"
#include "parser.h"

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
	
	return returnStatus;
}