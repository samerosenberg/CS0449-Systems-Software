#include "malloc.h"

typedef struct Node
{
  int blocksize;
  char free;
  struct Node  *next;
  struct Node *prev;
} Node;

Node * head = NULL;
Node * tail = NULL;

Node *find_free_Node(int size){
  Node *curr = head;
  Node *largest = NULL;
  while(curr!=NULL && curr!=tail){
    if(largest==NULL){
      if(curr->blocksize >= size && curr->free==1){
        largest = curr;
      }
    }else if(curr->blocksize > largest->blocksize && curr->free==1){
      largest==curr;
    }
    curr = curr->next;
  }

  if(curr!=NULL && curr->blocksize >= size && curr->free==1){
    if(largest==NULL){
      largest==curr;
    }else if(curr->blocksize > largest->blocksize){
      largest == curr;
    }
  }

  return largest;
}

Node *request_space(Node * last,int size){
  Node *block;
  block = sbrk(0);
  int blocksize = size + sizeof(Node);
  sbrk(blocksize);
  if(last){
    last->next = block;
  }
  block->blocksize = blocksize;
  block->next = NULL;
  block->prev = last;
  block->free = 0;
  tail=block;
}


void * mymalloc(int size)
{

  Node * block;
  if(!head){
    //printf("here");
    //Whether this prints or not changes address of head????
    block = request_space(NULL,size);
    head = block;
    tail = head;
  }else{
    Node * last = tail;
    block = find_free_Node(size);
    if(!block){
      block = request_space(last,size);
    }else{
      block->free = 0;
    }
  }
  return (block+1);

}


void myfree(void * ptr){
  if(ptr == NULL) return;
  Node * curr = (Node *) ptr - sizeof(Node);
  Node * prev = curr->prev;
  Node * next = curr->next;
  if(curr->free || ptr==NULL) return;
  if(curr!=head && curr!=tail){
    if(prev!=NULL && prev->free==1){
      if(next!=NULL && next->free==1){
        //printf("prev and next\n");
        prev->blocksize += curr->blocksize + next->blocksize;
        prev->next = next->next;
        next->next->prev = prev;
      }else{
        //printf("prev\n");
        prev->blocksize += curr->blocksize;
        prev->next = next;
        next->prev = prev;
      }
    }
    if(next!=NULL && next->free == 1){
      //printf("next\n");
      curr->blocksize += next->blocksize;
      next->next->prev = curr;
      next = next->next;
      curr->free=1;
    }
    if(prev!=NULL && prev->free ==0 && next!=NULL && next->free==0){
      curr->free = 1;
    }
  }else{
    if(head==tail){
      sbrk(0-head->blocksize);
      head=NULL;
      tail=head;
    }else{
      if(curr==head){
        if(curr->next->free==1){
          if(curr->next==tail){
            sbrk(0-head->blocksize);
          }
          head->blocksize += head->next->blocksize;
          head->free = 1;
          head->next->next->prev = head;
          head->next = head->next->next;
        }else{
          head->free=1;
        }
      }
      if(curr == tail){
        if(tail->prev->free==1){
          if(tail->prev==head){
            printf("here");
            sbrk(0-tail->blocksize-tail->prev->blocksize);
            head=NULL;
            tail=head;
          }else{
            sbrk(0-tail->blocksize-tail->prev->blocksize);
            tail = tail->prev->prev;
            tail->next = NULL;
          }
        }else{
          sbrk(0-tail->blocksize);
          tail=tail->prev;
          tail->next = NULL;
        }
      }
    }

  }
}

void print_list(){
	struct Node *node=head;
	while(node!=tail){
    printf("%p -> ",node+sizeof(Node));
    node=node->next;
  }
  printf("%p\n",tail+sizeof(Node));


}
