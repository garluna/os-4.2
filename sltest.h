#ifndef SLTEST_H
#define SLTEST_H

#include "SortedList.h"

/**
* Sorted List Test Driver
*/

/** GLOBALS **/ 
// PARAMETERS
extern int num_threads; // Default 1 thread
extern int num_iterations; // Default 1 iteration
extern int num_lists;// Default 1 list 
extern int use_mutex; // Default 0 (false)
extern int use_spinlock; // Default 0 (false)
extern int insert_yield; // Default 0 (false)
extern int delete_yield; // Default 0 (false)
extern int search_yield; // Default 0 (false)

static char* mutex_key = "m";
static char* spinlock_key = "s";

// SORTED LIST 
extern SortedList_t *head; // Head of list
extern SortedListElement_t** elem_array; // Pointer to SortedListElement* array 
extern int num_elements;

extern pthread_t* threads;

SortedList_t* initialize_list(SortedList_t* head);
SortedListElement_t* initialize_element(SortedListElement_t* element);
char* generate_key();
void terminate();
void* list_func(void* iter);
#endif
