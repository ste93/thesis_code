#ifndef FIND_AND_PRINT
#define FIND_AND_PRINT
#include "header_cli.h"
#include "utils_tree.h"

//void print_tree( node * root );
void print_nodes_and_sectors();
void print_node(node * ptr);
void find_and_print(node * root, key_type key, bool verbose); 
void find_and_print_range(node * root, key_type range1, key_type range2, bool verbose); 
void find_range( node * root, key_type key_start, key_type key_end, bool verbose, std::list<record*> * returned_rec); 
node * find_leaf( node * root, key_type key, bool verbose );
record * find( node * root, key_type key, bool verbose );
void print_record(record * ptr);

#endif
