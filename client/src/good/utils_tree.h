#ifndef UTILS_TREE
#define UTILS_TREE
#include "header_cli.h"
#include "cli_server.h"
#include "aes.h"
void enqueue( node * new_node);
node * dequeue( void );
void serialize_info();
void deserialize_info();
unsigned char * serialize( void * to_serialize, bool is_record, int * len_end);
//char * serialize_record(record * rec_to_serialize);
void * deserialize(unsigned char * n, bool is_record,uint32_t len);
//record * deserialize_record(char * rec_serialized);
int height( node * root );
int path_to_root( node * root, node * child );
int cut( int length );
node * read_from_file(node * root, int num_fields);

#endif
