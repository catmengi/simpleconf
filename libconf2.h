#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#define _GNU_SOURCE
typedef struct entry
{
 char* key;
 char* value;
 struct entry * next;
}confentry_t;
typedef struct section
{
  char* name;
  struct section * next;
  confentry_t * entry; //service info for looping entries
  confentry_t * origentry;
}confsect_t;
typedef struct config
{
  char* name;
  confsect_t * section;
  confsect_t * origSect; //serivce info for list looping
  //struct config * next;     future support to store many configs in one conf_t?
}conf_t;
void removeChar(char *str, char garbage) {                 

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}
int createConfE(conf_t * conf)              
 {
    if(conf == NULL)
     conf = malloc(sizeof(conf_t));
    if(conf == NULL)
     return 1;
     conf->section = malloc(sizeof(confsect_t));
    if(conf->section == NULL)
     return 2;
    conf->origSect = conf->section;
    conf->section->next = conf->origSect;
    return 0; 
 }
int addSectNodeE(conf_t* conf, char* name)
{   
    conf->section->next = NULL;
    conf->section->next = malloc(sizeof(confsect_t));
    if(!conf->section->next)
     return 1;
    if(conf->section->name != NULL)              
     conf->section = conf->section->next;
    else 
     free(conf->section->next);
    conf->section->next = conf->origSect;
    conf->section->name = strdup(name);
    conf->section->entry == NULL;
    return 0;
}
int addEntryNode(conf_t* conf , char * key , char* value )
{
   if(conf->section->entry == NULL){
    conf->section->entry = malloc(sizeof(confentry_t));
       if(conf->section->entry == NULL)
         return 1;
    conf->section->origentry = conf->section->entry;
    conf->section->entry->next = conf->section->origentry;
    conf->section->entry->key = strdup(key);
    conf->section->entry->value = strdup(value);
    return 0;
   }
   conf->section->entry->next = NULL;
   conf->section->entry->next = malloc(sizeof(confentry_t));
   conf->section->entry = conf->section->entry->next;
   conf->section->entry->key = strdup(key);
   conf->section->entry->value = strdup(value);
   conf->section->entry->next = conf->section->origentry;
   return 0;
}
conf_t * loadConf(char* filename,conf_t * config)
{
  FILE * fp = fopen(filename,"r");
   if(!fp)
    exit(-1);
  //conf_t * config;
  //createConfE(config);
  char buffer[256];
  addSectNodeE(config,"default");
  while(fgets(buffer,256,fp))
   {
     for(int i = 0; i < strlen(buffer);i++)
     if(buffer[i] == '#')
     buffer[i] = '\0';
     removeChar(buffer,'\n');
      removeChar(buffer,' ');
      switch(buffer[0])
       {
         case '@':
          removeChar(buffer,'@');
          addSectNodeE(config,buffer);
          break;
         default:
          int i = 0; 
          for(i = 0; i < strlen(buffer); i++)
           if(buffer[i] == '=')
            buffer[i] = '\0';
          char* vbuffer = buffer + i;
          addEntryNode(config,buffer,vbuffer);
          break;
       }
   }
}
char* getKeyValue(conf_t * config, char* ssection , char* key)     //search for key in section(first search section) then return value of key in string
{
   char* section = strdup(ssection);
   removeChar(section,' ');
   confsect_t * Sstart = config->section;
   do{
    //printf("section to found :%s || real :%s || next :%s\n",section,config->section->name,config->section->next->name);
    if(strcmp(config->section->name,section) == 0)
     break;
    config->section = config->section->next;
   }while(config->section->next != Sstart);
   confentry_t * Estart = config->section->entry;
    if(!Estart)
    return NULL;
   do{
    //printf("key  to found :%s || real :%s \n",key,config->section->entry->key);
    if(strcmp(config->section->entry->key,key) == 0)
     return config->section->entry->value;
    config->section->entry = config->section->entry->next;
   }while(config->section->entry != Estart);
   return NULL;
 }
