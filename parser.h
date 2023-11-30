#include "freader.h"
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
