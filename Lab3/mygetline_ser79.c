#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// YOU FILL IN THE CODE BODY FOR THIS FUNCTION BELOW MAIN
char * mygetline( char **buffer,  FILE * instream ); // JUST A PROTOTYPE. FUNCTION BODY IS BELOW MAIN

// MAIN IS GIVEN AS-IS DO NOT MODIFY

int main( int argc, char *argv[] )
{

  // ALWAYS check arg before using argv

  if (argc < 2 )
  {
	  fprintf( stderr, "usage: ./%s <input filename>\n", argv[0] );
	  exit( EXIT_FAILURE );
  }

  FILE * infile = fopen( argv[1], "rt" );
  if (!infile)
  {
	  fprintf( stderr, "Cant open %s for read.\n", argv[1] );
	  exit( EXIT_FAILURE );
  }

  char *buffer=NULL; // no buffer (char array) has been allocated yet. Mygetline will allocate space to this ptr
  int lineNum=0;

  // Mygetline will only return a non null pointer if it read something into the buffer for you to print.
  // Otherwise it will return with a NULL in buffer to indicate there is no line to print.

  while ( mygetline( &buffer, infile ) ) //  while mygetline()   equvalent to    while mygetline != NULL
  {
	  printf("%2d: %s", ++lineNum, buffer );
	  free ( buffer ); // Since mygetline malloc'd a string, so you must free it before reusing that buffer
  }

  fclose( infile );

  return (EXIT_SUCCESS );
}


/* ------------------------------- F U N C T I O N   D E F I N I T I O N S ------------------------------ */



/* CHAR * MYGELTINE

   Takes a char ** and allocates a small array of chars. Then reads from the stream into
   the char array. If it does not get the entire line in the first read it repeats until it gets the whole
   line, doubling the array size each try. Once the entire string is read in the newline is preserved and
   null char terminator added after the newline.

   After this is done it returns the pointer to the buffer containing the string.

   Your input file MUST have a newline at the very end of the file or this function becomes a bit messy to write.

   buffer is the addr of  the buffer ptr. This buffer ptr needs to be modified
   each time the buffer is doubled. This is why we don't just pass in a copy
   of the buffer ptr but instead pass in the address of the buffer ptr (i.e. a char **).

   infile is a ptr to a FILE object that is assumed to be a text stream  that is opened and ready for reading using fgets()


   FYI: if your input file is not terminated by a newline then your logic becomes more complicated.
   In this case it is not enough to just teest the return value of fgets for NULL to know if you have
   reached EOF and are done reading. If there is not a newline at the end of your file then you must
   make a read attempt and let that read atempt fail before a call to feof() know that EOF has been
   encountered. Just becasue you have already read in the last value and have actually reached EOF
   does not mean feof()  will return true. You must attempt a read and that read fail because there
   is nothing more there. In the absence of a terminating \n char a failed failed read attempt is
   required to  sets the internal EOF flag to true. Then the next call to feof() will return true.
*/

char * mygetline( char **buffer,  FILE * infile )
{
  /* use calloc it will fill array with NULL chars. Thus first time you call strlen on the buffer
     it will return 0 instead of segfaulting.
  */

  int buffSiz = 100;
  *buffer = calloc( buffSiz, 1 );
  char *temp = calloc(buffSiz,1);

  do
  {
    temp = calloc(buffSiz,1);
    char * result = fgets(temp, buffSiz, infile);
    strcat(*buffer,temp);

    if(result == NULL){
      free(*buffer);

      return NULL;
    }
    else{
      if(strchr(*buffer,'\n')==NULL){
        memcpy(temp, *buffer, buffSiz);
        buffSiz = buffSiz+1;
        *buffer = realloc(*buffer, buffSiz);
        memcpy(*buffer,temp,buffSiz);

      }
      else{
        return *buffer;
      }
    }
	/* make a read with fgets and store the result of the call into a ptr var
	   i.e. char * result = fgets( ... you figure out what to put here .. );

	   Look at result value. if it's NULL then
			fgets did not find anything in the file to read into the buffer
			you must free the memory used by the buffer (give it back to the heap)
			now you should return NULL the caller so caller understands not to try to print the buffer

 		Otherwise if it's not NULL then fgets put something into the buffer. You must determine
 		whether fgets got the entire line into the buffer  or if fgets only got a part of the line into
 		the buffer.  You can determine this by testing the buffer for presence of '\n' char
 		(Ask TA abour strchr)

 		If you got the entire line then just return *buffer ( not buffer,  not **buffer )

 		If you did not get the full line then
 			realloc the buffer to be twice as big
 			go back to the top of the loop and read again
	*/

  } while (1);  // INFINITE LOOP - WE ONLY GET OUT BY RETURNING FROM WITHIN

} // END MYGETLINE
