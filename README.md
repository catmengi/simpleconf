# simpleconf
simple linked list based config load system

____________________________________________
SIMPLE DOCUMEMTATION
____________________________________________
conf_t * type used to bind config files
int createConfE(conf_t * conf)  - preinitilaze conf_t* var for work, must be called ONCE for every file; RETURN VALUES THAT ARE NON ZERO ARE ERRORS
int loadConf(char* filename, conf_t * conf) - require preinitilized conf_t, read "filename" to conf_t; RETURN VALUES THAT ARE NON ZERO ARE ERRORS
char* getKeyValue(conf_t* conf, char* section,char*key) - require conf_t with loaded file, will search in it for section "section" and in this section will search key-value pair by name, return string value, NULL if eror
