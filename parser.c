#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "parser.h"

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
				printf("Threads! With an optional argument of %s \n", optarg);
				break;

			case ITERATIONS:
				printf("Iterations! With an optional argument of %s \n", optarg);
				break;

			case LISTS:
				printf("Iterations! With an optional argument of %s \n", optarg);
				break;

			case YIELD:
				printf("Yield! With argument of %s \n", optarg);
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