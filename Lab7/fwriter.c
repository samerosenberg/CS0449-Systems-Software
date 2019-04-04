#include <stdio.h>
#include <pthread.h>

#define MAXTHREADS 4
#define SYMBOLS_PER_FILE 2000
char * symbols = "-#&@";
int numThreads=0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * writeFile(void * fname)
{
	pthread_mutex_lock(&mutex);
	int threadNum = numThreads++;
	pthread_mutex_unlock(&mutex);
	int writeCnt=0;
	FILE *f = fopen( (char*)fname, "wt" );
	if ( !f )
	{
		fprintf( stderr, "Thread %d unable to open file %s\n",threadNum, (char*)fname );
		pthread_exit( (void*) -1 ); // something BAD
	}
	while (writeCnt<SYMBOLS_PER_FILE)
	{
		char value = symbols[threadNum];
		fprintf(f, "%c", value);
		printf("%c", value);
		writeCnt++;
	}
	fclose( f);
	return (void*)writeCnt;
}

int main(int argc, char ** argv)
{
	void* readCnts[MAXTHREADS];
	pthread_t threads[MAXTHREADS];     // thread object
	for( int i=0 ; i<MAXTHREADS ; i++)
		pthread_create(&threads[i], NULL, writeFile, (void *)argv[i+1]);
	for( int i=0 ; i<MAXTHREADS ; i++)
		pthread_join( threads[i], &readCnts[i] );
	printf("\n");
	for (int i=0 ; i < MAXTHREADS ; ++i )
		printf( "Thread %d  wrote %d %c to file %s\n", i+1, (int)readCnts[i], symbols[i], argv[i+1] );

	return 0;
}
