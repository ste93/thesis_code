#ifndef CLI_SERVER
#define CLI_SERVER

#include "header_cli.h"
void *retrieveDataFake(int socketfd, sect_type sector, bool is_record, node * root, int root_index);
void * retrieveRoot(void);
int send_tree (node * root);

#endif
