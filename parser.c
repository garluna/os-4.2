#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "parser.h"
#include "sltest.h"

/** GLOBALS **/ 
int num_threads = 1;
int num_iterations = 1;
int num_lists = 1;

int character_to_int(char* c)
{
	int val; 
	char* end; 

	val = strtoll(c,&end,0);
	if(end == c) // Argument is not a digit
	{
		return -1;
	}

	return val;
}

int parse(int argc, char** argv)
{
	int c; // Argument code returned by getoptlong
	int option_index = 0; // Option index
	int loop = 1; 
	
	while(loop)
	{
		c = getopt_long (argc, argv, "", parameters, &option_index);
		
		if(c == -1) // No more options
			break; 
		switch(c)
		{
			case THREADS:
				num_threads = character_to_int(optarg);
				if (num_threads == -1)
				{
					fprintf(stderr, "ERROR: Argument is not a digit!");
					return 1;
				}
				printf("The number of threads is %d \n", num_threads);
				break;

			case ITERATIONS:
				num_iterations = character_to_int(optarg);
				if (num_iterations == -1)
				{
					fprintf(stderr, "ERROR: Argument is not a digit!");
					return 1;
				}	
				printf("The number of Iterations is %d \n", num_iterations);			
				break;

			case LISTS:
				num_lists = character_to_int(optarg);
				if (num_lists == -1)
				{
					fprintf(stderr, "ERROR: Argument is not a digit!");
					return 1;
				}	
				printf("The number of lists is %d \n", num_lists);	
				break;

			case YIELD:
				
				break;

			case SYNC: 
				printf("Sync! With argument of %s \n", optarg);
				break;

			case '?': // Unknown option or missing arg to option
				break; 

			default:
				abort();
		}
	}
	return 0;
}