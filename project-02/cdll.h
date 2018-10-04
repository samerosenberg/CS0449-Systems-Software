/*
	Interface for  Circular Doubly Linked List (templated)
	Original Author: Guna@cs.cmu.edu
	adapted extensively for Josephus problem by Thoffman@cs.cmu.edu
*/

#ifndef CDLL_H
#define CDLL_H

#define CLOCKWISE 1
#define COUNTERCLOCKWISE -1

#define BRIEF 0
#define FULL 1

/* -------------------------------------------------------------------------------------------------------
	CIRCULAR DOUBLY LINKED LIST NODE (templated)

	struct CDLL_NODE::  list element (Node) type - for a templated, circular, doubly linked list

	-templated: the data (key) field is void *,  meaning at runtime the data
	field can accept a ptr to ANY type - thus, this element can store ANY type

	-doubly linked: each element has both a next AND a prev pointer

	-circular: the last element (Node) of list points (via its "next" ptr) to the first elem in the list
*/

typedef struct CDLL_NODE
{
  void *data;              /* data val for node: might be int *, might be char *  */
  struct CDLL_NODE *next;  /* pointer to next node */
  struct CDLL_NODE *prev;  /* pointer to previous node */
} CDLL_NODE;

/*  ------------------------------------------------------------------------------------------------------
	CIRCULAR DOUBLY LINKED LIST CLASS (templated)

	struct CDLL::  list class containing the head pointer to the first CDLL_NODE and the pointer
	to the functions used to compare one Node's data  to another and print the data value and the Node

	Depending on the main program that main will pass a different compare function and print function
	as appropriate for the actual data type stored in the Node.
*/

typedef struct CDLL
{
  CDLL_NODE *head;     /* head: ptr to first Node in list  */
  int (*compare)(void*, void*);  /* addr of a compare function to be used on data values in two different Nodes */
  void (*print)( void *, int);           /* addr of a print function to be used on a data values in a Node */
  void (*freeData)(void *);

} CDLL;

void fatal( char * msg );

/*  ------------------------------------------------------------------------------------------------------
	initList:: The list (CDLL) struct has the ptr to the first Node, and has 2 function ptrs. The first function
	ptr is the compare function and the second is the print function. All 3 fields of the List
	struct must be initialized by setting head field to NULL and passing the addresses of
	2 actual functions for compare() and print()
*/
void initList(CDLL *list, int (*compare)(void*, void*), void (*print)(void*, int), void (*freeData)(void *));


/*  ------------------------------------------------------------------------------------------------------
	insertAtTail:: similar to the insertAtTail you know from normal linked lists. One significant
	difference is that the newly appended node must point BACK to the old tail and point forward
	(wrap-around) to the first node in the list.

	MUST reject dupes: print rejected dupe message and free the data object passed in - free the student struct that was created
	free what you are not going to store permananetly

*/
void insertAtTail(CDLL *list, void *data);

/*  ------------------------------------------------------------------------------------------------------
	deleteNode:: similar to the deleteNode you know from normal linked lists but it is a
	bit messier. You must be diligent to correctly modify the next pointer on the node behind
	the deadNode and correctly change the prev point on the node after the deadNode.
	The direction parameter determines which Node, either before or after deadNode to return
	a pointer to. If direction is CCW we return deadNode->prev ELSE deadNode->next.

	If the front node is deleted the head should then point to the NEXT of the node just deleted,
	regardless of which direction was passed in and regardless of what pointer is returned by the function.
*/
CDLL_NODE * deleteNode(CDLL *list, CDLL_NODE *deadNode, int direction );

/*  ------------------------------------------------------------------------------------------------------
	printList:: iterates thru the list calling the print() function on the data field of each node in the list.
	the print() function is stored in the CDLL struct that is passed in.

	brief = 0 mode => print some
	full = 1 mode => print ALL!

	iterate through the list => print the data in brief mode

	comparison is based on the andrewID => determines uniqueness
*/
void printList( CDLL list, int direction, int mode );

/*  ------------------------------------------------------------------------------------------------------
	searchList:: iterates thru the list looking for a node whose data field matches the passed in target.
	searchList uses the compare() function stored in the CDLL struct which is passed in as first arg.
	Returns a ptr to the node containing the target or returns NULL if not found.
*/
CDLL_NODE * searchList( CDLL list, void * target );


#endif /* CDLL_H */
