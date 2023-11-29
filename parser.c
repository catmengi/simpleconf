#include "freader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct raw_string_llist_e
{
  char raw_str[MAX_READ_STR_LEN];
  struct raw_string_llist_e* next;
};
struct raw_string_llist    /*dirty hacks for unprivatizing private module)*/
{
  struct raw_string_llist_e * head;
  int ecount;
};

typedef union
{
  long ceuid;
  int section_id;
  int entry_id;
}ceuid;

typedef struct _conf_entry
{
    ceuid ceuid;
    char* key;
    char* value;
    struct _conf_entry* next;
}_conf_entry_t;
typedef struct _conf_sect
{
    ceuid ceuid;
    char* key;
    struct _conf_sect * next;
    _conf_entry_t* entry_head;
}_conf_sect_t;

struct config
{
  _conf_sect_t* head;
};
char* strdup_s(char* str)
{
    if(!str)
        return NULL;
    char* new_str = malloc(strlen(str));
    if(!new_str)
        return NULL;
    memset(new_str,0 , strlen(str));
    strcpy(new_str,str);
    return new_str;
}
void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}
int rs_llist_parse(raw_str_llist_t* rs_llist,struct config* config_out)
{
    if(!rs_llist)
        return 1;
    if(!config_out)
        return 1;
    config_out->head = malloc(sizeof(_conf_sect_t));
    if(!config_out->head)
        return 2;
    _conf_sect_t* cur_sect = config_out->head;
    if(cur_sect->key != NULL)
        return 1;
    cur_sect->key = strdup_s("default");
    if(!cur_sect->key)
        return 2;
    struct raw_string_llist* raw_string = rs_llist; /*unprivatize hack*/
    struct raw_string_llist_e* r_cur = raw_string->head;
    while(r_cur!=NULL)
    {
      removeChar(r_cur->raw_str,'\n');
      removeChar(r_cur->raw_str,' ');
      char* comm_remove = index(r_cur->raw_str,'#');
      if(comm_remove)
          *comm_remove = '\0';
      if(r_cur->raw_str[0] == '[')
      {
       char tmp_buff[strlen(r_cur->raw_str)];
       strcpy(tmp_buff,r_cur->raw_str+1);
       for(int i = 0; i<strlen(tmp_buff);i++)
       {
           if(tmp_buff[i] == ']')
           {
            *index(tmp_buff,']') = '\0';
            cur_sect->next = malloc(sizeof(_conf_sect_t));
            if(!cur_sect->next)
                return 2;
             if(cur_sect->key != NULL)
                 cur_sect = cur_sect->next;
             else
                free(cur_sect->next);
             cur_sect->key = strdup_s(tmp_buff);
            if(!cur_sect->key)
                return 2;
            printf("tmp_buff %s\n",tmp_buff);
            goto cont_rl;
           }
       }
      }
    cont_rl:
    r_cur = r_cur->next;
     continue;
    }
    return 0;
}
