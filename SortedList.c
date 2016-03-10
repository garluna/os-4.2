#include <string.h>
#include <pthread.h>
#include <stdio.h>
#define _GNU_SOURCE

#include "SortedList.h"

#if defined(__APPLE__) || defined(__MACH__) 
#define pthread_yield() pthread_yield_np()
#endif

/** GLOBALS **/ 
int insert_yield; // Default 0 (false)
int delete_yield; // Default 0 (false)
int search_yield; // Default 0 (false)
int use_mutex; // Default 0 (false)
int use_spinlock; // Default 0 (false)

int opt_yield = 0;

// LOCKS
static pthread_mutex_t lock_m; // mutex lock
volatile static int lock_s; // spin lock 
/*
 * SortedList (and SortedListElement)
 *
 *	A doubly linked list, kept sorted by a specified key.
 *	This structure is used for a list head, and each element
 *	of the list begins with this structure.
 *
 *	The list head is in the list, and an empty list contains
 *	only a list head.  The list head is also recognizable because
 *	it has a NULL key pointer.
 */

 void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
 {

 	if(use_mutex && !use_spinlock){
 		printf("USE MUTEX \n");
 		pthread_mutex_lock(&lock_m);
 	}

 	SortedListElement_t *p = list; // Initialized at list head
 	SortedListElement_t *n = list->next; // The first element
 	
 	// Find the right position to insert the swipe 
 	while(n != list)
 	{
 		if (strcmp(element->key, n->key) <= 0)
 			break;

 		p = n;

 		n = n->next;
 	
 	}
 	
 	// OPT_YIELD
 	if(insert_yield)
 		opt_yield = INSERT_YIELD;
 	if(opt_yield & INSERT_YIELD){
 		printf("YIELDING! \n");
 		pthread_yield();
 	}
 	// Perform the insertion 
 	element->prev = p;
 	element->next = n;
 	p->next = element;
 	n->prev = element;
 	
 	 if(use_mutex && !use_spinlock) // ONLY USE MUTEX LOCK
 		pthread_mutex_unlock(&lock_m);
	
 }


 SortedListElement_t* SortedList_lookup(SortedList_t *list, const char *key)
 {
 	SortedListElement_t *iter = list->next; // Initialized at first element
 	while(iter != list)
 	{
 		if (search_yield)
 			opt_yield = SEARCH_YIELD;
 		if (opt_yield & SEARCH_YIELD)
 			pthread_yield();
 		if(strcmp(iter->key, key) == 0) // Found Element
 			return iter;

 		iter = iter->next;
 	}
 	return NULL; // Unable to find element 
 }

 int SortedList_delete(SortedListElement_t *element)
 {
  	if(use_mutex && !use_spinlock) // ONLY USE MUTEX LOCK
 		pthread_mutex_lock(&lock_m);

 	SortedListElement_t *p = element->prev;
 	SortedListElement_t *n = element->next;

 	if(p->next != element || n->prev != element)
 		return 1;

 	n->prev = p; 
 	p->next = n; 

 	// OPT_YIELD
 	if(delete_yield)
 		opt_yield = DELETE_YIELD;
 	if(opt_yield & DELETE_YIELD)
 		pthread_yield();

 	element->next = NULL;
 	element->prev = NULL;

  	if(use_mutex && !use_spinlock) // ONLY USE MUTEX LOCK
 		pthread_mutex_unlock(&lock_m);

 	return 0;
 }	

 int SortedList_length(SortedList_t *list)
 {
 	int counter = 0;
 	SortedListElement_t *p = list;
 	SortedListElement_t *n = list->next;

 	while (n != list)
 	{
 		// Check for pointer corruption 
 		if (n->prev != p)
 			return -1; 
 		if (p->next != n)
 			return -1; 

 		// OPT_YIELD
 		if(search_yield)
 			opt_yield = SEARCH_YIELD;
 		if(opt_yield & SEARCH_YIELD)
 			pthread_yield();
 		counter++;
 		p = n;
 		n = n->next;
 	}

 	return counter;
 }
