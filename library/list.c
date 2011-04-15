#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Simple two-way LIFO */

typedef struct sListNode{
  struct sListNode *next,*prev;
  void *data;
}listNode;

typedef struct sList{
  listNode *head,*tail;
  int size; 
  int elementSize;
}list;

/* Public inteface */
void    listInit      (list *l, int elementSize);
void    listPushBack  (list *l, const void *x);
void    listPopBack   (list *l, void *x);
void    listPushFront (list *l, const void *x);
void    listPopFront  (list *l, void *x);
void    listDestroy   (list *l);
int     listEmpty     (list *l);
void    prerror       (char *s);
/* END of Public interface */

/* Definitions */

void listInit(list *l, int elementSize) {/*{{{*/
  /* Allocate memory for the head/tail guards, set size to, set elementSize 0*/

  if ( (l->head = (listNode*) malloc(sizeof(listNode))) == NULL ) perror("Mem alloc fail");
  if ( (l->tail = (listNode*) malloc(sizeof(listNode))) == NULL ) perror("Mem alloc fail");

  l->head->next = l->tail; l->tail->prev = l->head;
  l->size = 0;
  l->elementSize = elementSize;
}/*}}}*/

void listPushBack(list *l, const void *x) {/*{{{*/
  /* Allocate memory for a new element, allocate place for data, memcpy from x to data, increase list size */
  listNode  *tmp;
  char      *buffer;

  if ( (tmp     = (listNode*) malloc(sizeof(listNode))) == NULL ) perror("Mem alloc fail (listPushBack,tmp)");
  if ( (buffer  = (char*)     malloc(l->elementSize))   == NULL ) perror("Mem alloc fail (listPushBack,buffer)");

  memcpy(buffer, x, l->elementSize);

  tmp->prev = l->tail->prev;
  tmp->data = (void*)buffer;
  tmp->next = l->tail;

  l->tail->prev->next = tmp;
  l->tail->prev       = tmp;

  l->size++;
}/*}}}*/

void listPopBack(list *l, void *x) {/*{{{*/
  if ( listEmpty(l) ) prerror("list empty");

  listNode *tmp;
  
  tmp = l->tail->prev;

  l->tail->prev->prev->next = l->tail;
  l->tail->prev = l->tail->prev->prev;

  memcpy(x, tmp->data, l->elementSize);

  free(tmp);

  l->size--;
}/*}}}*/

void listPushFront(list *l, const void *x) {/*{{{*/
  /* Allocate memory for a new element, allocate place for data, memcpy from x to data, increase list size */
  listNode  *tmp;
  char      *buffer;

  if ( (tmp     = (listNode*) malloc(sizeof(listNode))) == NULL ) perror("Mem alloc fail (listPushBack,tmp)");
  if ( (buffer  = (char*)     malloc(l->elementSize))   == NULL ) perror("Mem alloc fail (listPushBack,buffer)");

  memcpy(buffer, x, l->elementSize);

  tmp->prev = l->head;
  tmp->data = (void*)buffer;
  tmp->next = l->head->next;

  l->head->next->prev = tmp;
  l->head->next       = tmp;

  l->size++;
}/*}}}*/

void listPopFront(list *l, void *x) {/*{{{*/
  if ( listEmpty(l) ) prerror("list empty");

  listNode *tmp;
  
  tmp = l->head->next;

  l->head->next->next->prev = l->head;
  l->head->next             = l->head->next->next;

  memcpy(x, tmp->data, l->elementSize);

  free(tmp);

  l->size--;
}/*}}}*/

void listDestroy(list *l) {/*{{{*/
  listNode *x,*y;

  x = l->head; y = x->next;
  while ( y != NULL ) {
    free(x);
    x = y;
    y = y->next;
  }
  free(x);
}/*}}}*/

int listEmpty(list *l) {/*{{{*/
  return l->size == 0;
}/*}}}*/

void prerror(char *s) {/*{{{*/
  printf("%s\n",s);
  exit(0);
}/*}}}*/

/* END of definitions */

int main() {
  list l;
  list *lp;
  int i,q,w,e;

  lp = &l;

  listInit(lp, sizeof(int));

  q = 14;

  for (i=0;i<10;i++) {
    listPushFront(lp, &i);
    listPushBack(lp, &i);
  } 
  for(i=0;i<21;i++) {
    listPopBack(lp, &w);
    printf("%d\n",w);
  }
  listDestroy(lp);

}

