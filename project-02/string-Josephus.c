#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdll.h"


char * mygetline( char **buffer,  FILE * infile );

void printString( void * data, int mode )
{
	// YOUR CODE HERE
}

int compareString( void * data1, void *data2 )
{
	// YOUR CODE HERE
	return 0;  // just to make it compile you change as needed 
}

/* Reads strings from the file into the liked list */
void loadList( CDLL *list, char *infileName)
{
	FILE* infile = fopen(infileName, "r");
	if(!infile) fatal("Can't open input file");

	char *name;
	while( mygetline( &name, infile ) )
		insertAtTail( list, name );
	fclose(infile);
}

void freeString(void *data)
{
	// YOUR CODE HERE
}

/* == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == */
int main(int argc, char *argv[])
{
	CDLL list;
	int i,direction, k;
	CDLL_NODE *currNode;

	if (argc < 4)
	{
		printf("You must supply 3 values on the cmd line after %s:\n", argv[0]);
		printf("1: name of an input file contaning n names/labels of the members of the circle\n");
		printf("2: an integer:  k < n  ,  indicating that after deleting the selected starting element, k members are skipped before another deletions until all the members are deleted\n");
		printf("3: a string \"CW\" or \"CCW\" for clockwise counter-clockwise. This is the direction\n");
		printf("   you will move as you advance around the ring deleting members\n");
		exit( EXIT_FAILURE );
	}

	k  = atoi(argv[2]); /* assume valid int < n */
	if (strcmp(argv[3],"CW")==0)
		direction = CLOCKWISE; /* follow the NEXT ptr */
	else
		direction = COUNTERCLOCKWISE; /* follow the PREV ptr */

	/* Initialize CDLL (set head ptr NULL and set the cmp function */
	initList(&list, compareString,printString,freeString);
	loadList(&list,argv[1]);
	printf("\nLIST: ");printList(list, CLOCKWISE, BRIEF ); /* we follow the NEXT ptr around the ring, not the PREV ptr */

	do /* prompt user for a member to start the counting with */
	{
		char *name;
		printf("\nchoose a name as the starting point in the circle: ");
		if (!mygetline( &name,stdin)) fatal("mygetline failed in main reading from stdin.");
		currNode = searchList( list, name); // if user enters non-existent name it will ret NULL
		free( name );
	} while (!currNode);

	while (list.head) /* while list has any members left */
	{
		printf("\nDELETING: "); printString( currNode->data, BRIEF); fflush(stdout);
		currNode = deleteNode( &list, currNode, direction );
		if (!currNode)
		{
			printf("  <-- was the last man standing!\n");
			break;
		}
		printf("\nLIST: "); fflush(stdout);
		printList(list, CLOCKWISE, BRIEF );
		printf("RESUMING AT: "); printString( currNode->data, BRIEF ); printf( " and skipping %d elements \n", k );
		for ( i=1 ; i<=k ; ++i )
			if (direction==CLOCKWISE)
				currNode = currNode->next;
			else
				currNode=currNode->prev;

	} /* END WHILE - END OF GAME */
    return EXIT_SUCCESS;
}

// - - - - - - - - - - - - - - -  H E L P E R   F U N C T I O N S - - - - - - - - - - - -

char * mygetline( char **buffer,  FILE * infile )
{
  int buffSiz = 10;						// this line was given
  *buffer = calloc( buffSiz, 1 );      // this line was given

  do
  {
    	char * result = fgets( *buffer+strlen(*buffer), buffSiz-strlen(*buffer), infile);
	    if (!result)
    	{
			free( *buffer ); // must destroy wasted buffer and return NULL. Don't want caller to attempt to print this buffer
			return NULL;
		}
    	if ( strchr( *buffer, '\n' ) )
    	{
			strtok( *buffer, "\n");  // chomp
			return *buffer; // if '\n' in buffer we got the whole line
		}

    	// Didn't get entire line yet.  Need to double our buffer and read again

    	buffSiz *= 2;
    	*buffer = realloc( *buffer, buffSiz );

  } while (1);  // INFINITE LOOP - WE RETURN OUT

} // END MYGETLINE
