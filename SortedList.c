#include <string.h>
#include "SortedList.h"
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

 	// Perform the insertion 
 	element->prev = p;
 	element->next = n;
 	p->next = element;
 	n->prev = element;
 }


 SortedListElement_t* SortedList_lookup(SortedList_t *list, const char *key)
 {
 	SortedListElement_t *iter = list->next; // Initialized at first element
 	while(iter != list)
 	{
 		if(strcmp(iter->key, key) == 0) // Found Element
 			return iter;

 		iter = iter->next;
 	}
 	return NULL; // Unable to find element 
 }

 int SortedList_delete(SortedListElement_t *element)
 {
 	SortedListElement_t *p = element->prev;
 	SortedListElement_t *n = element->next;

 	if(p->next != element || n->prev != element)
 		return 1;

 	n->prev = p; 
 	p->next = n; 

 	element->next = NULL;
 	element->prev = NULL;
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

 		counter++;
 		p = n;
 		n = n->next;
 	}

 	return counter;
 }