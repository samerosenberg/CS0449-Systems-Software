#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdll.h"

typedef struct STUDENT
{
	char * andrewID;     /* i.e. weirdal */
	char * name;         /* i.e. Alfred Yankovic */
	int yrOfGrad;        /* i.e 2010 */
	double gpa;          /* i.e 4.0 */
	char * major;        /* i.e CS  */
} STUDENT;

char * mygetline( char **buffer,  FILE * infile );
char ** split(char * line, int * tokensCnt, char * delim);

// if mode == BRIEF just print the ID filed followed by a space  (no newline)
// if not BRIEF print ID name yrOfGrad gpa major with a space in between  (no newline)

void printStudent( void * data, int mode )
{
	// YOUR CODE HERE
}

// see the strcmp function defintion - do likewise
int compareStudent( void * data1, void *data2 )
{
	// YOUR CODE HERE
	// do your strcmp operation baed on the id string
}

// you are given a ptr to the node to be freed
void freeStudent(void *student)
{
	//YOUR CODE HERE
	// free the srtring fields
	// then free the node itself
}


/* Reads strings from the file into the liked list */
void loadList( CDLL *list, char *infileName)
{
	char *line;
	char ** tokens;
	int tokenCnt=0;
	char * delim = ",";

	FILE* infile = fopen(infileName, "r");
	if(!infile) fatal("Can't open input file");

	while( mygetline( &line, infile ) )
	{
		STUDENT *s = malloc(sizeof(STUDENT));
		if (!s) fatal("malloc( sizeof(STUDENT))  failed in loadList");

		tokens = split( line, &tokenCnt, delim );
		s->andrewID = tokens[0];
		s->name = tokens[1];
		s->yrOfGrad = atoi(tokens[2]);
		s->gpa = atof(tokens[3]);
		s->major = tokens[4];

		insertAtTail( list,  s );

		free(line);
		free(tokens[2]);      /* year */
		free(tokens[3]);      /* major */
		free(tokens);
	}
	fclose(infile);
}

/* == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == */
int main(int argc, char *argv[])
{
	CDLL list;
	int i,direction,k;
	CDLL_NODE *currNode;

	if (argc < 4)
	{
		printf("You must supply 3 values on the cmd line after %s:\n", argv[0]);
		printf("1: name of an input file contaning n names/labels of the members of the circle\n");
		printf("2: an integer:  m < n  ,  indicating that every m'th member gets deleted until all the members are deleted\n");
		printf("3: a string \"CW\" or \"CCW\" for clockwise counter-clockwise. This is the direction\n");
		printf("   you will move as you advance around the ring deleting members\n");
		exit( EXIT_FAILURE );
	}

	k = atoi(argv[2]); /* assume valid int < n */
	if (strcmp(argv[3],"CW")==0)
		direction = CLOCKWISE; /* follow the NEXT ptr */
	else
		direction = COUNTERCLOCKWISE; /* follow the PREV ptr */

	/* Initialize CDLL (set head ptr NULL and set the cmp function */
	initList(&list, compareStudent,printStudent,freeStudent);
	loadList(&list,argv[1]);
	printf("\nLIST: ");printList(list, CLOCKWISE, BRIEF ); /* we follow the NEXT ptr around the ring, not the PREV ptr */

	do /* prompt user for a member to start the counting with */
	{
		char *andrewID;
		STUDENT *s = malloc(sizeof(STUDENT));
		if (!s) fatal("malloc( sizeof(STUDENT))  failed in main");

		currNode=NULL;
		printf("\nchoose an ID as the starting point in the circle: ");
		fflush(stdout);
		if (!mygetline(&andrewID,stdin)) fatal("mygetline failed on stdin.");
		s->andrewID = andrewID;
		currNode = searchList( list, s);

		/* Can't call freeStudent() because it will free the name and major string but we didnt put any in this one -
		    this student only has the id field initilaized. so manually just free the andereID string then node*/
		free( s->andrewID );
		free( s );

	} while (!currNode);

	while (list.head) /* WHILE THERE ARE ANY MEMBERS STILL ALIVE */
	{
		printf("\nDELETING: "); printStudent( currNode->data, BRIEF); fflush(stdout);
		currNode = deleteNode( &list, currNode, direction );
		if (!currNode)
		{
			printf("  <-- was the last man standing!\n");
			break;
		}

		printf("\nLIST: ");printList(list, CLOCKWISE, BRIEF );
		printf("RESUMING AT: "); printStudent( currNode->data, BRIEF ); printf(" and skipping %d elements \n", k );
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


char ** split(char * line, int * tokensCnt, char * delim)
{
	//printf("SPLIT: %s\n", line );
	int tokensCap=1; /* initial capacity */
	char ** tokens=NULL; /* the token array */
	char  * token;   /* ptr to first token and then each succesive token each time strtok called */
	*tokensCnt=0;    /* how many of those ptrs in use by a string */

	tokens = malloc(tokensCap * sizeof(*tokens));
	if (!tokens) fatal("Initial malloc in split failed. Program Aborting");
	if (!line)
	{
		printf("Line passed into split was NULL\n");
		free(tokens); /* ony 1 ptr but we gotta free it */
		return NULL;
	}
	token=line; /* ptr to the first token */
	token = strtok(line, delim);
	while (token)
	{
		if (*tokensCnt == tokensCap)
		{
			tokens = realloc( tokens, (++tokensCap) * sizeof(*tokens) );
			if (!tokens) fatal("realloc in split failed. Program Aborting");
		}
		tokens[*tokensCnt] = malloc(strlen(token)+1);
		strcpy( tokens[*tokensCnt], token );
		++(*tokensCnt);
		token = strtok( NULL, delim);
	}
	return tokens;
}