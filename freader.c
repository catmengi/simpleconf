#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_READ_STR_LEN 256
struct raw_string_llist_e
{
  char raw_str[MAX_READ_STR_LEN];
  struct raw_string_llist_e* next;
};
struct raw_string_llist
{
  struct raw_string_llist_e * head;
  struct raw_string_llist_e* cur;/*for get api*/
  int ecount;
};
struct raw_string_llist* rs_llist_create()
{
  struct raw_string_llist* out = malloc(sizeof(struct raw_string_llist)) ;
  if(!out)
      return NULL;
  return out;
}
int fread_llist(FILE* fp,struct raw_string_llist* out)
{
  if(!fp)
      return 1;
  if(!out)
  {
    return 2;
  }
  if(!out->head)
  {
    out->head = malloc(sizeof( struct raw_string_llist_e));
     if(!out->head)
         return 2;
  }
  char buffer[MAX_READ_STR_LEN];
  struct raw_string_llist_e * cur = out->head;
  while(fgets(buffer,MAX_READ_STR_LEN,fp))
  {
    if(!cur)
        return 4;
    cur->next = malloc(sizeof(struct raw_string_llist_e));
    if(!cur->next)
        return 4;
    if(cur->raw_str[0] != '\0' || cur->raw_str != NULL)
       cur = cur->next;
    else
       free(cur->next);
    strncpy(cur->raw_str,buffer,MAX_READ_STR_LEN);
  }
  cur = out->head;
  while(cur != NULL)
  {
    (out->ecount)++;
    cur = cur->next;
  }
  return 0;
}
void* imalloc(size_t n,int v)
{
    void* mem = malloc(n);
    if(!mem)
        return NULL;
    memset(mem,v,n);
    return mem;
}
