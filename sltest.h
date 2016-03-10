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
extern int num_elements;
extern SortedListElement_t** elem_array; // Pointer to SortedListElement* array 

extern pthread_mutex_t lock_m;

// SUBLISTS 
typedef struct sublist
{
	SortedList_t* sub_head; 
	int lock_index; 
} sublist_t; 

extern sublist_t* sublists; 
extern pthread_t* threads;

extern pthread_mutex_t* mutexes;
extern int* volatile locks; 

SortedList_t* initialize_list(SortedList_t* head);
SortedListElement_t* initialize_element(SortedListElement_t* element);
char* generate_key();
void terminate();
void* list_func(void* iter);
void* sublist_func(void* index);
int hash_key(const char* key);
#endif
