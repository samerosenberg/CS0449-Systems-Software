#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define malloc(x) my_worstfit_malloc(x)
#define free(x) my_free(x)

void * sbrk( int increment );
void * my_worstfit_malloc (int size );
void my_free (void *ptr);
void heapWalk( char * caption);    // YOU FINISH THIS METHOD BELOW MAIN

void *base; // The original base of the HEAP as given to us by the compiler

int main ()
{
	// Save this so you know where the absolute start of the heap is every time
	// you want to traverse the heap looking for a block.

	base = sbrk(0); // DO NOT MODIFY. THIS IS YOUR ORIGINAL BASE Of THE HEAP

	// MALLOC 40 bytes then free it
	int * arr1 = malloc( 10 * sizeof(int) ); // Note payload is even number of bytes
	free( arr1 );

	// MALLOC 50 bytes do NOT free
	char * string1= malloc( 50  ); // Note payload is even number of bytes
	strcpy( string1, "Hey World! I just malloced 50 bytes!" );
	printf("%s\n", string1 );

	// MALLOC 20 bytes then free it
	int * arr2 = malloc( 5 * sizeof(int) ); // Note payload is even number of bytes
	for( int i= 0 ; i<5 ; ++i ) arr2[i] = i*2;
	for( int i= 0 ; i<5 ; ++i )
		printf( "%d ", arr2[i] );
	printf("\n");
	free( arr2 );

	// MALLOC 30 bytes do NOT free
	char * string2= malloc( 30  ); // Note payload is even number of bytes
	strcpy( string2, "I hate malloc!" );
	printf( "%s\n\n", string2 );

	heapWalk("STATE OF HEAP AFTER 4 MALLOCS, 2 FREES");

	return 0;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// MY_WORSTFIT_MALLOC

// I'ts not really a worstfit scheme. It's worse than worse. Every call to malloc appends
// a new block to the top of the heap. We use the heap like a stack that never pops :(
// NO re-use of freed blocks. No coalescing. Very wastefull.
// Allocates space for footer but does not initialize footer (last 4 bytes of the block)

void * my_worstfit_malloc (int size )
{
	int blocksize = size + 2*sizeof(int);      // payload + header + footer
	int * blockstart = (int *) sbrk(0);  	   // similar to stack save our local frame base ptr
	sbrk( blocksize );   						 	   // similar to an activation record making itself room on the stack
	// the 4 bytes at blockstart is the header which serves as both count and status
	*blockstart = blocksize | 0x00000001;   // set high order bit to 1 == IN USE
	return blockstart+1;		                    // return start addr of payload (right after 4 byte hdr)
}

void my_free (void *ptr)
{
	int * blockstart = ((int*)ptr) - 1;
	*blockstart &= 0xFFFFFFFE;                // set low bit to 0 ( block is UN-USED / FREE)
}

// YOU MUST FINISH THIS FUNCTION
void heapWalk( char * caption)
{
	if (base == sbrk(0))  // base == top means no blocks (in use or freed) exist on the HEAP
	{
		printf("The size of the heap is 0. There no allocated blocks in the heap.\n");
		return;
	}

	// If you are here then there must have been some blocks allocated on the heap. Assume the first one start at the first 4 bytes of the heap

	int blockNum=0;  // counter for which block we are one  1,2,3,4,5 etc.
	char * blockstart = base;  // mem addr if first byte in heap
	int      baseoffset = 0;     // since we are at the very base of HEAP, the bytes offest is 0
	
	printf("   HEAPBASE: %d\t(where first block starts under this 'appendage' allocation scheme)\n", (int)base );
	printf("BLOCK# STATUS STARTADDR\tBASE_OFFS PAYLOAD    HDR/FTR\tBLOCKSIZE\n");
	do
	{
		++blockNum;

		// STUDENTS MUST WRITE FILL IN BELow
		// 1: grab the first four byes pointed to by blockstart. Store it in an int. Call it 'blockheader'. That is your current block's header
		int blockheader =0;
		memcpy(&blockheader,blockstart,sizeof(int));
		//printf("%d\n",blockheader);
		// 2: examine/isolate the low order rightmost bit in that int. This is your STATUS: 0=FREE,  1=USED bit.
		int stat = blockheader & 1;
		char * status= "";
		if(stat == 0){
			status = "FREE";
		}else{
			status = "USED";
		}
		// 3: examine that blockheader int again but mask off the status bit and extract into an int called 'blocksize'.
		int blocksize = blockheader&0xFFFFFFFE;
		// 4: compute the offset of this block as distance in bytes from original heap base to your blockbase. Your first block's offset will be 0
		//baseoffset = 48*(blockNum-1);
		// 5: compute payload as being blocksize - 4 bytes HEADER - 4 bytes FOOTER (look at malloc function. That's what we did)
		int payload = blocksize - 2*sizeof(int);
		// 6: print block# status startadd baseoffset payload hdr/ftr  blocksize  (see output and read main malloc calls)
		//printf("%d  %s",(int)base,blockstart);
		printf("%d\t%s %d\t%d\t %d bytes   8 bytes\t%d\n",blockNum,status,(int)blockstart,baseoffset,payload,blocksize);

		// now set things up to find/decode the next block

		// 7: advance  blockstart ptr to  addr of first byte beyond the end of this block.
		//	  (ptr + int   i.e. add blocksize to the blockbase pointer)
		//     This also makes it possible to know when to stop looking for the next block.
		//     See  bottom of this loop.
		blockstart = blockstart + blocksize;

		// 8: increment baseoffset by this block's blocksize  (int + int i.e. add blocksize to baseoffest i.e.  )
		baseoffset = baseoffset + blocksize;
		// Now you are set up to find the next block with your base ptr pointing at the first byte of the next block

		// END OF SOLUTION SECTION

	} while ( (void *)blockstart < sbrk(0) );  // While we have not reached TOP of the HEAP

	printf("    HEAP TOP: %d\t(where next block would start under this 'appendage' allocation scheme)\n", (int) sbrk(0) );
}
