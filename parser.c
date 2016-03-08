#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "parser.h"
#include "sltest.h"

/** GLOBALS **/ 
int num_threads = 1; // Default 1 thread
int num_iterations = 1;
int num_lists = 1;
int use_mutex = 0;
int use_spinlock = 0; 
static char* mutex_key;
static char* spinlock_key;
int insert_yield; // Default 0 (false)
int delete_yield; // Default 0 (false)
int search_yield; // Default 0 (false)

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

int parse_yield(char* arg)
{
	int i;
	for(i=0; i < strlen(arg); i++)
	{
		switch(arg[i])
		{
			case 'i':
				insert_yield = 1; 
				break;

			case 'd':
				delete_yield = 1;
				break;

			case 's':
				search_yield = 1;
				break;

			case '\0':
				break;

			default: // Argument contains a character that is not i, d, or s
				return -1; 
		}
	}
	return 0;
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
				if(parse_yield(optarg) == -1)
				{
					fprintf(stderr, "ERROR: Argument is not [ids]");
					return 1;
				}
				break;

			case SYNC: 
				if(strcmp(optarg,mutex_key) == 0)
				{
					use_mutex = 1;
					printf("Use pthread_mutex \n");
				}
				else if(strcmp(optarg, spinlock_key) == 0)
				{
					use_spinlock = 1;
					printf("Use spinlocks! \n");
				}
				else // Invalid option
				{
					fprintf(stderr, "ERROR: %s is an invalid option! Use m for mutexes or s for spinlocks \n", optarg);
					return 1; 
				}
				break;

			case '?': // Unknown option or missing arg to option
				break; 

			default:
				abort();
		}
	}
	return 0;
}