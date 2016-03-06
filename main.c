#include <stdlib.h>
#include <stdio.h>

#include "SortedList.h"
int main(int argc, char** argv)
{
	SortedList_t temp; 
	SortedListElement_t temp_element; 
	char test[2]= {'a', '\0'};
	temp_element.key = test;
	SortedList_insert(&temp, &temp_element);

	printf("Pinatas are cool! The size of the list is: %d", SortedList_length(&temp));
	return 0;
}