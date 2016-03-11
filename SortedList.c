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
int num_lists; 
int opt_yield = 0;

// LOCKS FOR SINGLE LIST
pthread_mutex_t lock_m; // mutex lock
volatile int lock_s = 0; // spin lock 

// LOCKS FOR SUBLIST
pthread_mutex_t* mutexes;
int* volatile locks; 

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

 	if(num_lists == 1) // LOCKS FOR SINGLE LSIT
 	{
 		if(use_mutex) // MUTEX
 		{
	 		pthread_mutex_lock(&lock_m);
 		}

 		if(use_spinlock) // SPIN
	 	{
	 		while(__sync_lock_test_and_set(&lock_s, 1))
			{
				continue;
			}
	 	}
 	}

 	SortedListElement_t *p = list; // Initialized at list head
 	SortedListElement_t *n = list->next; // The first element
 	
 	if (!list || !element || !p || !n)
 		return;

 	// Find the right position to insert the swipe 
 	while(n != list)
 	{
 		if(!n){
	 		if(num_lists == 1) // LOCKS FOR SINGLE LSIT
	 		{
		 		if(use_mutex) // MUTEX
		 		{
			 		pthread_mutex_unlock(&lock_m);
		 		}

		 		if(use_spinlock) // SPIN
			 	{
					__sync_lock_release(&lock_s);
			 	}
 			}
 			return;
 		}
 		if (strcmp(element->key, n->key) <= 0)
 			break;

 		p = n;

 		n = n->next;
 	
 	}
 	
 	// OPT_YIELD
 	if(insert_yield){
 		opt_yield = INSERT_YIELD;
 	}
 	if(opt_yield & INSERT_YIELD){
 		pthread_yield();
 	}
 	// Perform the insertion 
 	element->prev = p;
 	element->next = n;
 	p->next = element;
 	n->prev = element;
 	
 	if(num_lists == 1)
 	{
	 	if(use_mutex) // ONLY USE MUTEX LOCK
	 		pthread_mutex_unlock(&lock_m);

		if(use_spinlock) // USE SPIN LOCK
	 	{
			__sync_lock_release(&lock_s);
	 	}
	}
	
 }


 SortedListElement_t* SortedList_lookup(SortedList_t *list, const char *key)
 {
 	if (!list || !key || !list->next)
 	{
 		printf("[lookup] Pointer value in lookup (passed in list) is %p \n", list);
 		return NULL;
 	}

 	SortedListElement_t *iter = list->next; // Initialized at first element
 	while(iter != list)
 	{
 		if (!iter)
 		{
 			printf("[lookup] Pointer value in lookup (iter) is %p \n", iter);
 			return NULL; 
 		}

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
 	if(num_lists == 1)
 	{
	  	if(use_mutex) // ONLY USE MUTEX LOCK
	 		pthread_mutex_lock(&lock_m);

	 	if(use_spinlock)
	 	{
	 		while(__sync_lock_test_and_set(&lock_s, 1))
			{
				continue;
			}
	 	}
	}

 	// check for NULL pointers
 	if(!element || !element->next || !element->prev)
 	{
	 	if (num_lists == 1)
	 	{
		  	if(use_mutex) // ONLY USE MUTEX LOCK
		 		pthread_mutex_unlock(&lock_m);

		  	if(use_spinlock)
		 	{
				__sync_lock_release(&lock_s);
		 	}
		}
	 	// printf("[delete] Pointer value in delete (passed in element) is %p \n", element);
 		return 1;
 	}

 	SortedListElement_t *p = element->prev;
 	SortedListElement_t *n = element->next;

 	if(p->next != element || n->prev != element)
 	{
	  	if (num_lists == 1)
	 	{
		  	if(use_mutex) // ONLY USE MUTEX LOCK
		 		pthread_mutex_unlock(&lock_m);

		  	if(use_spinlock)
		 	{
				__sync_lock_release(&lock_s);
		 	}
		}
 		// printf("[delete] Pointer value in delete for p is: %p and n is: %p \n",p, n);
 		return 1;
 	}

 	n->prev = p; 
 	p->next = n; 

 	// OPT_YIELD
 	if(delete_yield)
 		opt_yield = DELETE_YIELD;
 	if(opt_yield & DELETE_YIELD)
 		pthread_yield();

 	element->next = NULL;
 	element->prev = NULL;

 	if (num_lists == 1)
 	{
	  	if(use_mutex) // ONLY USE MUTEX LOCK
	 		pthread_mutex_unlock(&lock_m);

	  	if(use_spinlock)
	 	{
			__sync_lock_release(&lock_s);
	 	}
	}

 	return 0;
 }	

 int SortedList_length(SortedList_t *list)
 {
 	int counter = 0;

 	if(!list || !list->next || !list->prev)
 	{
 		// printf("[length] Pointer value for passed in list in length is %p \n", list);
 		return -1;
 	}

 	SortedListElement_t *p = list;
 	SortedListElement_t *n = list->next;

 	while (n != list)
 	{
 		if(!n || n->prev != p || p->next != n )
 		{
 			// printf("[length] Pointer value in length is %p \n", n);
 			return -1;
 		}

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
