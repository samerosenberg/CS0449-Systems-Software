#include <stdio.h>
#include <pthread.h>

#define BUFLEN 8000
#define MAX_THREADS 4
char buffer[BUFLEN];
int bufCnt=0;
int numThreads=0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * readFile(void * fname)
{
	int offset = numThreads*2000;
	pthread_mutex_lock(&mutex);
	int threadNum = numThreads++; // global variable !!!
	pthread_mutex_unlock(&mutex);
	int readCnt=0;
	char symbol='?';
	FILE *f = fopen( (char*)fname, "rt" );
	if ( !f )
	{
		fprintf( stderr, "Thread %d unable to open file %s\n",threadNum, (char*)fname );
		pthread_exit( (void*) -1 ); // something BAD
	}
	printf("thread %d reading file %s into buffer\n", threadNum, (char*) fname);
	while (fscanf(f, "%c",&symbol) == 1 )
	{
		printf("%c",symbol);
		buffer[offset+readCnt++]=symbol;
		pthread_mutex_lock(&mutex);
		++bufCnt;  // global variable !!!
		pthread_mutex_unlock(&mutex);
	}
	return (void*)readCnt;
}

int main(int argc, char ** argv)
{
	void* readCnts[MAX_THREADS];
	pthread_t threads[MAX_THREADS];     // thread objects

	for( int i=0 ; i<MAX_THREADS ; i++)
		pthread_create(&threads[i], NULL, readFile, (void *)argv[i+1]);
	for( int i=0 ; i<MAX_THREADS ; i++)
		pthread_join( threads[i], &readCnts[i] );

	int each = BUFLEN/MAX_THREADS;
	printf("DUMPING BUFFER: Should be %d '-' then %d '#' then %d '&' then %d '@'\n", each,each,each,each );
	for (int i=0 ; i < bufCnt  ; ++i )
		printf("%5d:%c\n", i, buffer[i]);
	printf( "\n");
	for (int i=0 ; i < MAX_THREADS ; ++i )
		printf( "Thread %d  read in %d symbols from file %s\n", i+1, (int)readCnts[i] , argv[i+1] );
	return 0;
}
