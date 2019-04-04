#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//include your code
#include "mymalloc.h"

int main(){

	void * a = mymalloc(10);
	printf("a allocated %p\n",a);
	void * b = mymalloc(30);
	printf("b allocated %p\n",b);

	myfree(b); printf("b freed\n");


	void * c = mymalloc(20);
	printf("c allocated %p\n",c);
	void * d = mymalloc(10);
	printf("d allocated %p\n",d);
	void * e = mymalloc(10);
	printf("e allocated %p\n",e);

	print_list();

	//change ordering to check each case for freeing
	myfree(a); printf("a freed\n");

	myfree(e); printf("e freed\n");

	myfree(d); printf("d freed\n");

	myfree(c); printf("c freed\n");
	return 1;




}
