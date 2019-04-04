/*
mystrings program for project 3

MY NAME: Sam Rosenberg
MY PITT EMAIL: ser79@pitt.edu

*/




#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE * inFile = NULL;

  if (argc < 2)
    {
      fprintf(stderr,"\nMust specify a file on the command line. Please try again.\n");
      exit(EXIT_FAILURE);
    }
  if  ((inFile = fopen(argv[1],"rb")) == NULL)
    {
      fprintf(stderr,"Can't open %s for input\n", argv[1] );
      exit(EXIT_FAILURE);
    }

    char string[100] = "";
    char byteIn;
    int count =0;


    while(fread(&byteIn,1,1,inFile)==1){
      if(byteIn>=32 && byteIn<=126){
        string[count] = byteIn;
        count++;
      }else if(string[3]!=0){
        count++;
        string[count] = '\0';
        printf("%s\n", string);
        memset(&string[0],0,sizeof(string));
        count =0;
      }
    }


  fclose(inFile);
  return EXIT_SUCCESS;
}
