#ifndef PARSER_H
#define PARSER_H

#include <getopt.h>
/**
* parameters
*
* The available options for sltest. 
*/

#define THREADS 1
#define ITERATIONS 2
#define LISTS 3
#define YIELD 4
#define SYNC 5

static struct option parameters[] =
{
	{"threads", 	optional_argument, 0, THREADS    },
	{"iterations",	optional_argument, 0, ITERATIONS },
	{"lists",		optional_argument, 0, LISTS      },
	{"yield",		required_argument, 0, YIELD      },
	{"sync",		required_argument, 0, SYNC       }
};

/**
 * parse ... parse command line options
 *
 * @param int argc ... the number of arguments
 * @param char** argv ... array of arguments
 *
 * @return #: exit status of sltest operations
 */
int parse(int argc, char** argv);

/**
* char_to_int ... converts char to int using strtoll
* 
* @param char* c ... string to be converted
*
* @return converted integer; -1 if the conversion is not a digit
*/ 
int character_to_int(char* c);
#endif 