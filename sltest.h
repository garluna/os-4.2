#ifndef SLTEST_H
#define SLTEST_H

/**
* Sorted List Test Driver
*/

/** GLOBALS **/ 
extern int num_threads; // Default 1 thread
extern int num_iterations; // Default 1 iteration
extern int num_lists;// Default 1 list 
extern int use_mutex; // Default 0 (false)
extern int use_spinlock; // Default 0 (false)

static char* mutex_key = "m";
static char* spinlock_key = "s";
#endif