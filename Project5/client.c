//ser79	  9250-9259

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
FILE *output;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * readFile(void * threadPort){
  int con_fd = 0;
  int ret = 0;
  struct sockaddr_in serv_addr;
  char buffer[1024];

  int block = 0;
  char blockreq[128];
  unsigned short port= atoi(threadPort);
  char * file_name = NULL;
  int done = 1;
  con_fd = socket(PF_INET, SOCK_STREAM, 0);

  while(done==1){
    con_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (con_fd == -1) {
      perror("Socket Error\n");
      pthread_exit( (void*) -1 );
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(con_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));

    if (ret < 0) {
      //perror("Connect error\n");
      pthread_exit( (void*) -1 );
    }

  //  printf("%d",block);
    memset(buffer, '\0', sizeof(buffer));
    sprintf(blockreq,"%d",block);

    //pthread_mutex_lock(&mutex);
    send(con_fd, blockreq,128,0);

    int valread = recv(con_fd, buffer,1024,0);
    //pthread_mutex_unlock(&mutex);
    if(valread==0){
      fprintf(output,"%s\n",buffer);
      done =0;
      pthread_exit( (void*) -1 );
    }

    printf("%s\n",buffer);
    fprintf(output,"%s\n",buffer);

    block++;


  }
  close(con_fd);

}

int main(int argc, char **argv){
  if (argc <= 2) {
    printf("Usage: ./client <ippaddr1> <port>\n");
    return -1;
  }

  output = fopen("output.txt","w");

  pthread_t threads[argc-1/2];     // thread objects


	for( int i=1 ; i<sizeof(threads)/sizeof(threads[0])-1; i=i+2){
		pthread_create(&threads[i], NULL, readFile, (void *)argv[2*i]);
  }
  for( int i=1 ; i<sizeof(threads)/sizeof(threads[0])-1 ; i++)
    pthread_join( threads[i], NULL );


  return 0;

}
