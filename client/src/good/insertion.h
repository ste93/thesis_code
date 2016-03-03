#ifndef INSERTION
#define INSERTION
#include "find_and_print.h"
#include "cli_server.h"
#include "creation.h"

int get_left_index(node * parent, node * left);
node * insert_into_leaf( node * leaf, key_type key, record * pointer );
node * insert_into_leaf_after_splitting(node * root, node * leaf, key_type key, record * pointer);
node * insert_into_node(node * root, node * parent, int left_index, key_type key, node * right);
node * insert_into_node_after_splitting(node * root, node * parent, int left_index, key_type key, node * right);
node * insert_into_parent(node * root, node * left, key_type key, node * right);
node * insert_into_new_root(node * left, key_type key, node * right);
node * start_new_tree(key_type key, record * pointer);
node * insert( node * root, key_type key, fields_type * fields );

#endif
