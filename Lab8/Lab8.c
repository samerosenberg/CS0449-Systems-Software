#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[]) {
  int sfd;
  struct sockaddr_in addr;

  sfd = socket(PF_INET, SOCK_STREAM,0);
  if(sfd<0){
    printf("Socket not working");
    exit(0);
  }

  memset(addr.sin_zero, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9250);
  addr.sin_addr.s_addr = INADDR_ANY;


  if(bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) <0){
      printf("Bind not working");
      exit(0);
    }

  if(listen(sfd,10)<0){
    printf("Listen not working");
    exit(0);
  }

  int connfd = accept(sfd,NULL,NULL);
  if(connfd<0){
    printf("Accept not working");
    exit(0);
  }

  char buffer[1024];
  strcpy(buffer, "Hello there!\n");
  send(connfd, buffer, strlen(buffer), 0);

  close(sfd);


  return 0;
}
