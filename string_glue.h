#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
char* strglue(const char* s1, const char* s2)
{
  size_t bstrlen = strlen(s1) + strlen(s2);
  char* gstr = malloc(bstrlen);
  if(!gstr)
   return NULL;
  memset(gstr,0,bstrlen);
  strcpy(gstr,s1);
  gstr += strlen(s1);
  strcpy(gstr,s2);
  gstr -=strlen(s1);
  return gstr;
}