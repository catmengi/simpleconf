# simpleconf
simple linked list based config load system

____________________________________________
SIMPLE DOCUMEMTATION
____________________________________________
conf_t * type used to bind config files;

int createConfE(conf_t * conf)  - preinitilaze conf_t* var for work, must be called ONCE for every file; RETURN VALUES THAT ARE NON ZERO ARE ERRORS;

int loadConf(char* filename, conf_t * conf) - require preinitilized conf_t, read "filename" to conf_t; RETURN VALUES THAT ARE NON ZERO ARE ERRORS;

char* getKeyValue(conf_t* conf, char* section,char*key) - require conf_t with loaded file, will search in it for section "section" and in this section will search key-value pair by name, return string value, NULL if eror;

char** getSections(conf_t* config) - return NULL if error or return array of string containing all sections founded in file on success

int confsectlen(conf_t* config) - return -1 on error or return ammount of sections in config

int getEntryAmm(conf_t* config, char* section) - return -1 on error or return ammount of key-value pairs in section on success

char** getEntryKEys(conf_t*, char* section) - return NULL on error or return array of strings containing all keys finded in section on success

int sectScrollTo(conf_t* config,char* section) return -1 on fail, return 0 on success, moves current config->section to section name position

int conffree(conf_t* config) - return 0 on success return -1 on error, free config, sections,entrys and etc;

there some internal usage functions that can be used for something eg genSectPointer, see in code how to use they

!!!MULTI STRING NOT SUPPORT (key-value pair where value or equals sign are on other line)!!!


_________________________
FILE EXAMPLE
_________________________
cfgname=example;config  # this is not array, just string with semicolon, your configs must started with this(cfgname=) or parser will ignore this config

[maybe it is full ini?] #if you need to access section staticly know about that that spaces are deleted in stage of parsing

this is = are entry or key value pair # same thing spaces will be deleted
