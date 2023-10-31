#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>

//#define _GNU_SOURCE
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
  confentry_t * entry;
  confentry_t * origentry; //service info looping lists;
}confsect_t;
typedef struct config
{
  char* name;
  confsect_t * section;
  confsect_t * origSect; //serivce info for list looping
}conf_t;
int conffree(conf_t*);
char* getKeyValue(conf_t*,char*,char*);
void removeChar(char *str, char garbage) {                 

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}
int createConfE(conf_t * conf)              //MUST called before loadConf
 {
    if(!conf)
     conf = malloc(sizeof(conf_t));
    if(!conf)
     return 1;
    conf->section = malloc(sizeof(confsect_t));
    if(!conf->section)
     return 2;
    conf->origSect = conf->section;
    conf->section->next = conf->origSect;
    return 0; 
 }
static int addSectNodeE(conf_t* conf, char* name)          //adding node to sections, ONLY FOR loadConf usage!
{   
    conf->section->next = NULL;
    conf->section->next = malloc(sizeof(confsect_t));
    if(!conf->section->next)
     return 1;
    if(conf->section->name)
     conf->section = conf->section->next;
    else 
     free(conf->section->next);
    conf->section->next = conf->origSect;
    conf->section->name = strdup(name);
    conf->section->entry = NULL;
    return 0;
}
static int addEntryNode(conf_t* conf , char * key , char* value )   //adding node to entry in current section, ONLY FOR loadConf USAGE, DOESNT SUPPORT SECTION CHANGING
{
   if(conf->section->entry == NULL){
    conf->section->entry = malloc(sizeof(confentry_t));
       if(!conf->section->entry)
         return 1;
    conf->section->origentry = conf->section->entry;
    conf->section->entry->next = conf->section->origentry;
    conf->section->entry->key = strdup(key);
    conf->section->entry->value = strdup(value);
  if(!conf->section->entry->key || !conf->section->entry->value)
    return -1;
  return 0;
   }
   conf->section->entry->next = NULL;
   conf->section->entry->next = malloc(sizeof(confentry_t));
   if(!conf->section->entry->next)
    return 1;
   conf->section->entry = conf->section->entry->next;
   conf->section->entry->key = strdup(key);
   conf->section->entry->value = strdup(value);
   if(!conf->section->entry->key || !conf->section->entry->value)
    return 1;
   conf->section->entry->next = conf->section->origentry;
   return 0;
}
int loadConf(char* filename,conf_t * config)   //loads up config(read data from file and generating sections and entry, support comments and inline comments via '#)
{
  FILE * fp = fopen(filename,"r");
   if(!fp)
    return -1;

  createConfE(config);
  char buffer[256];
  if(addSectNodeE(config,"default"))
   return 1;
  while(fgets(buffer,256,fp))
   {
     for(int i = 0; i < strlen(buffer);i++)
      if(buffer[i] == '#')
        buffer[i] = '\0';
     if(buffer[0] == '\n')
      continue;
     removeChar(buffer,'\n');
     removeChar(buffer,' ');
     int i = 0;
     switch(buffer[0])
      {
        case '[':
         if(buffer[strlen(buffer)-1] != ']')
          break;
         removeChar(buffer,'[');
         removeChar(buffer,']');
         if(addSectNodeE(config,buffer))
          return 1;
         break;
        case '#':
         break;
        case ' ':
         break;
        default:
         for(i = 0; i < strlen(buffer); i++)
          if(buffer[i] == '=')
           buffer[i] = '\0';
         char* vbuffer = buffer + i;
         if(addEntryNode(config,buffer,vbuffer))
          return 1;
         break;
      }
   }
   char* tmpcfg=getKeyValue(config,"default","cfgname");
   if(tmpcfg != NULL)
   {
     config->name = malloc(strlen(tmpcfg));
     strcpy(config->name,tmpcfg);
   }
   else
   {
    conffree(config);
    free(tmpcfg);
    fclose(fp);
    return -1;
   }
  free(tmpcfg);
  fclose(fp);
  return 0;
}
int sectScrollTo(conf_t* config, char* section)      //searching section with name 'section' by scrolling the list. if founded return 0 else -1
{
  if(section == NULL)
  {
    return -1;
  }
  if(!config)
    return -1;
  if(!config->section)
    return -1;
  confsect_t * Ssect = config->section;
 do{
    if(strcmp(section,config->section->name) == 0)
    {
      return 0;
    }
  config->section = config->section->next;
  }while (config->section != Ssect);
  return -1;
}
char* getKeyValue(conf_t * config, char* ssection , char* key)     //search for key in section(first search section) then return value of key in string
 {
    char* section = strdup(ssection);
    if(section == NULL){
      return NULL;
    }
    removeChar(section,' ');
    if(sectScrollTo(config,section) == -1){
      free(section);
      return NULL;
    }
   confentry_t * Estart = config->section->entry;
    if(!Estart){
    free(section);
    return NULL;
    }
   do{
    if(config->section->entry == NULL){
      free(section);
     return NULL;
    }
    if(strcmp(config->section->entry->key,key) == 0){
     free(section);
     return config->section->entry->value;
    }
    config->section->entry = config->section->entry->next;
   }while(config->section->entry != Estart);
   free(section);
   return NULL;
 }
int confsectlen(conf_t* config)
{
  int len = 0;
  if(!config)
    return -1;
  if(!config->section)
    return -1;
  if(!config->section->next)
    return -1;
  confsect_t* Sstart = config->section;
   do{
      len++;
      config->section = config->section->next;
     }while(config->section != Sstart);
     //printf("%d\n",len);
  return len;//conf_t * config;
}
char** getSections(conf_t * config)    //creates string array containing all sections in config;
{ 
  int len = confsectlen(config);
  if(len == -1)
    return NULL;
  char** sectList = malloc(len * sizeof(char*));
  if(!sectList){
    perror("pre sectList malloc() failed\n");
     return NULL;
  }
  for(int i = 0; i < confsectlen(config); i++)
   {
    sectList[i] = (char*)malloc(strlen(config->section->name));
    if(sectList[i] == NULL){
     perror("sectList malloc() failed\n");
     free(sectList);
      return NULL;
    }//conf_t * config;
    strcpy(sectList[i],config->section->name);
   // printf("DEBUG : %s\n",sectList[i]);
    config->section = config->section->next;
   }
  return sectList;
}
int getEntryAmm(conf_t* config,char* section)  //return ammount of entryes in section 'section'
{
  if(sectScrollTo(config,section) == -1){
    //printf("SCROLL ERROR!!!!\n");
    return -1;
  }
  int len = 0;
  confentry_t * Sentry = config->section->entry;
   if(!Sentry){
    return -1;
   }
   do
   {
      len++;
      config->section->entry = config->section->entry->next;
   }while (config->section->entry != Sentry);
  return len;
}
char** getEntryKeys(conf_t* config, char* section)     // list all keys in section 'section';
{
  if(sectScrollTo(config,section) == -1)
  {
    printf("SCROLL ERROR\n");
     return NULL;
  }
  int len = getEntryAmm(config,section);
  char** entryarr = malloc(sizeof(char*) * len);
  confentry_t * Sentry = config->section->entry;
  if(!Sentry)
  {
    printf("SENTRY %p\n",Sentry);
    return NULL;
  }
  int i = 0;
  do
  {
    entryarr[i] = malloc(strlen(config->section->entry->key));
    if(!entryarr[i])
    {
      perror("malloc() in getEntryKeys\n");
      exit(-1);
    }
    strcpy(entryarr[i],config->section->entry->key);
    i++;
    config->section->entry = config->section->entry->next;
  }while(config->section->entry != Sentry);
  return entryarr;
}
confsect_t ** genSectPointers(conf_t* config)    //generates the pointer array that contains pointers to ALL sections
{
  int len = confsectlen(config);
   if(!len)
     return NULL;
  confsect_t ** sectptr = malloc(sizeof(confsect_t*) * len);
  if(!sectptr){
    return NULL;
  }
  for(int i = 0; i < len; i++){
    sectptr[i] = config->section;
    config->section = config->section->next;
  }
  return sectptr;
}
confentry_t ** genEntryPointers(conf_t* config, char* section)   //same as genSectPointer but creates pointers for entrys in section 'section'
{
  if(sectScrollTo(config,section) == -1)
    return NULL;
  int len = getEntryAmm(config,section);
  if(!len)
    return NULL;
  confentry_t ** entryptr = malloc(len * sizeof(confentry_t*));
  if(!entryptr)
    return NULL;
  for(int i = 0; i < len ; i++){
    entryptr[i] = config->section->entry;
    config->section->entry = config->section->entry->next;
  }
  return entryptr;
}
int conffree(conf_t* config)                     //free almost all config (conf_t) with sections and entrys in this sections
{
   char** sections = getSections(config);
   int sectlen = confsectlen(config);
   int entrylen;
   if(!sections)
   {
     return -1;
   }
   confsect_t** sectptr = genSectPointers(config);
   if(!sectptr)
   {
     return -1;
   }
   confentry_t ** entryptr;
   for(int i = 0; i < sectlen; i++)
   {
     entrylen = getEntryAmm(config,sections[i]);
     entryptr = genEntryPointers(config,sections[i]);
     if(entryptr == NULL){
       //printf("entry in current section is NULL\n");
       goto entryskip;
    }
    for(int j = 0; j < entrylen; j ++)
    {
     free(entryptr[j]->key);
     if(entryptr[j]->value != NULL)
         free(entryptr[j]->value);
     free(entryptr[j]);
    }
    free(entryptr);
   entryskip:
   if(sectptr[i]->name != NULL)
      free(sectptr[i]->name);
  }
//  free(entryptr);
  for(int i = 0; i < sectlen; i++)
  {
    //printf("clearing section %s\n ",sectptr[i]->name);
    free(sections[i]);
    free(sectptr[i]);
  }
  free(sectptr);
  free(sections);
  free(config->name);
  free(config);
  return 0;
}
