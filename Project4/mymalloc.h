// This head file just do the definitation, all implementation will be in corresponding c file.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
//define the malloc and my free

void * mymalloc(int size);
void myfree(void *ptr);

// define other function you may need for the linked list
void *find_free_Node(void **last,int size);
void *request_space(void * last,int size);
void print_list();
