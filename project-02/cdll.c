#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdll.h"

/* just a convenience */
void fatal( char * msg )
{
	printf("%s\n",msg);
	exit(EXIT_FAILURE);
}


/* ----------------------------------------------------------------------------
	initList:

*/
void initList(CDLL *list, int (*compare)(void*, void*), void (*print)(void*, int),void (*freeData)(void *))
{
	list->head = NULL;
	list->compare = compare;
	list->print = print;
	list->freeData = freeData;

//	1) set the head pointer in the CDLL struct to NULL
//	2) assign each of the incoming function pointers into their respective fields in the CDLL struct
}


/* ----------------------------------------------------------------------------
*/
void insertAtTail(CDLL * list, void *data)
{
	while(list->head->next!=NULL){
		list->head = list->head->next;
	}
	CDLL_NODE * new = malloc(sizeof(CDLL_NODE));
	list->head->next = new;
	new->data = data;
	new->prev = list->head;
}



/* ----------------------------------------------------------------------------
	deleteNode:

	You have  passed in the pointer to the node to be deleted.
	No need to iterate or search. Of course a check for a NULL pointer passed in
	would not hurt.
	Delete the deadNode then return the pointer to it's successor (if CW) or
	if you are going CCW then return pointer to the deadNode's predecessor.
	If deadnode was the last node then return NULL since there is no succ or pred.
*/
CDLL_NODE * deleteNode(CDLL *list, CDLL_NODE *deadNode, int direction )
{
	if(list->head!=NULL && strcmp((list->head->data),deadNode->data)==0){
		list->head = list->head->next;
		return list->head->next;
	}
	while(list->head!=NULL &&  strcmp((list->head->data),deadNode->data)!=0){
		list->head->prev = list->head;
		list->head = list->head->next;
	}
	if(list->head == NULL) return NULL;
	list->head->prev = list->head->next;
	return list->head->next;
}



/* ----------------------------------------------------------------------------
	printList:

	Observe my solution executable to see how it should look
	You are really just writing the loop and calling the printData on each node
*/

void printList( CDLL list, int direction, int mode )
{
	while(list.head!=NULL){
		if(direction == CLOCKWISE){
			char *s = (char*)(list.head->data);
			printf("%s",s);
			list.head = list.head->next;
		}
		else{
			char *s = (char*)(list.head->data);
			printf("%s",s);
			list.head = list.head->prev;
		}
	}
}



/* ----------------------------------------------------------------------------
	searchList:

	Scan list until you find a node that contains the data value passed in.
	If found return that pointer - otherwise return NULL
*/
CDLL_NODE * searchList( CDLL list, void * target )
{
	char * key = (char*)target;
	if(list.head==NULL){
		return NULL;
	}
	else{
		while(list.head != NULL){
			if(strcmp(list.head->data,key)==0){
				return list.head;
			}
			else{
				list.head = list.head->next;
			}
		}
		return NULL;
	}
}
