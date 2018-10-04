/*
  project-1.c

  MY NAME: Sam Rosenberg
  MY PITT EMAIL: ser79@pitt.edu

  As you develop and test this file:

  use this command to compile: (you can name the executable whatever you like)
  gcc -W -Wall -Wextra -O2   project-1.c   -o project-1.exe

  use this command to execute:  (you will of course test on both input files)

  ./project-1.exe  sine-1.bin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[])
{
  FILE * inFile= NULL;
  short int min,max,startVal,endVal,prevVal,currentVal;
  int currentNum,prevNum,samples, startNum,endNum; int started=0;

  // DECLARE ALL ADDITIONAL NEEDED VARIABLES

  /* recommend declaring two short int i.e prev & current so that you can compare
     them to look for zero crossings or changes in direction and sign.
     You will also need some int counters etc.
  */

  if (argc < 2)
    {
      fprintf(stderr,"\nMust specify a binary file on the command line. Please try again.\n");
      exit(EXIT_FAILURE);
    }
  if  ((inFile = fopen(argv[1],"rb")) == NULL)
    {
      fprintf(stderr,"Can't open %s for input\n", argv[1] );
      exit(EXIT_FAILURE);
    }

  // YOUR CODE HERE - READ EACH VALUE FROM THE BINARY FILE ONE AT A TIME AND LOOK FOR ZERO CROSSINGS TO DETECT WAVES

  /* recommended strategy:
     read in the first value of the file (prev) before the loop starts.
     Then in the while loop read next short in.
     In the loop you are to be looking for critical events:  zero crossings or sign changes.
     Before you start writing that code start out just echoing each value and the sample # associated with it.
     Once that is right start printing an alert whenever zero is touched/crossed OR or the direction changes.
     Zero crossings and direction changes are the critical events you must correctly detect.
     Once that is right then try to detect the start of the first wave.
     Once that is right add code to  detect the end of the first wave.
     Then start coding to detet every wave - counting samples per wave and
     keeping track of the highest and lowest value in that wave.
  */
    fread(&prevVal,sizeof(prevVal),1,inFile);
    currentNum=1;
    prevNum=0;
    if(prevVal==0){
      startNum=currentNum;
      started=1;
    }
      //printf("%hi\n",prev);
    while(fread(&currentVal,sizeof(currentVal),1,inFile)==1){
      //printf("%hi\n",current);
      currentNum++;
      prevNum++;
      if(currentVal==0 && prevVal<0){
        samples = currentNum-startNum+1;
        startNum = currentNum;
        endNum = currentNum;
        startVal = currentVal;
        endVal=currentVal;
        if(started==1){
          printf("%d\t %hi\t %d\t %hi\t %hi\n",endNum, endVal,samples,max,min );
          min=0;
          max=0;
        }
        started=1;
      }
      else if(currentVal>=0 && prevVal<0){
        samples = prevNum-startNum+1;
        startNum=currentNum;
        endNum=prevNum;
        startVal=currentVal;
        endVal=prevVal;
        if(started==1){
          printf("%d\t %hi\t %d\t %hi\t %hi\n",endNum, endVal,samples,max,min );
          min=0;
          max=0;
        }
        started=1;
      }


      if(currentVal>max){
        max=currentVal;
      }
      if(currentVal<min){
        min=currentVal;
      }
      prevVal = currentVal;
    }




  fclose(inFile); /* after the read loop is done. close file */

  return EXIT_SUCCESS;  // this is just a zero value. In C a zero return often indicates no error
}
