#include <stdio.h>
typedef struct raw_string_llist raw_str_llist_t;
#define MAX_READ_STR_LEN 256
/*
 *
struct raw_string_llist_e
{
  char raw_str[MAX_READ_STR_LEN];
  struct raw_string_llist_e* next;
};
struct raw_string_llist
{
  struct raw_string_llist_e * head;
  int ecount;
};
 *
 *
 you can use this structs to unprivatize raw_str_llist_t for you module;
*/
raw_str_llist_t* rs_llist_create(void);
int fread_llist(FILE*,raw_str_llist_t*);
void* imalloc(size_t,int);
