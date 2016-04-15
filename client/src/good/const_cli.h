#ifndef CONST_CLI
#define CONST_CLI
#define Version "1"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring>

/* this I think is not necessary, but maps the old sector with the new sector */
//std::map<uint32_t, uint32_t> old_and_new_sector;

/* here I have to put all the sectors retrieved in a set and then 
 * use it for reassign the sectors fro old to new.
 */



#define SOCKET_ERROR   ((int)-1)
#define INPSIZE 100
#define DISK "/dev/loop0"
#define SECTOR_SIZE 512
// Default order is 4.
#define DEFAULT_ORDER 20
#define NUM_VALUES 3
#define LEN_VALUES 15
// Minimum order is necessarily 3.  We set the maximum
// order arbitrarily.  You may change the maximum order.
#define MIN_ORDER 3
#define MAX_ORDER 20

// Constants for printing part or all of the GPL license.
#define LICENSE_FILE "LICENSE.txt"
#define LICENSE_WARRANTEE 0
#define LICENSE_WARRANTEE_START 592
#define LICENSE_WARRANTEE_END 624
#define LICENSE_CONDITIONS 1
#define LICENSE_CONDITIONS_START 70
#define LICENSE_CONDITIONS_END 625

typedef uint32_t sect_type;

typedef int32_t key_type;

typedef key_type fields_type;


/* this contains the information that must be stored in the first sector of
 * the server hard drive and contains the sector of the root and the last sector used in the hard drive
 */

typedef struct info {
	sect_type root_sect;
	sect_type last_sect_used;
	uint32_t num_fields;
} info;

/* Type representing a node in the B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 * The heart of the node is the array
 * of keys, the array of corresponding
 * pointers and the array of sectors.  The relation between keys
 * and pointers differs between leaves and
 * internal nodes.  In a leaf, the index
 * of each key equals the index of its corresponding
 * pointer, with a maximum of order - 1 key-pointer
 * pairs.  
 * 
 * 
 * The last pointer points to the
 * leaf to the right (or NULL in the case
 * of the rightmost leaf).
 * 
 * 
 * 
 * In an internal node, the first pointer
 * refers to lower nodes with keys less than
 * the smallest key in the keys array.  Then,
 * with indices i starting at 0, the pointer
 * at i + 1 points to the subtree with keys
 * greater than or equal to the key in this
 * node at index i.
 * The num_keys field is used to keep
 * track of the number of valid keys.
 * In an internal node, the number of valid
 * pointers is always num_keys + 1.
 * 
 * 
 * not correct for my data structure implementation
 * In a leaf, the number of valid pointers
 * to data is always num_keys.  The
 * last leaf pointer points to the next leaf.
 */
typedef struct node {
	key_type key;
	key_type keys[DEFAULT_ORDER];
	void * pointers[DEFAULT_ORDER];
	sect_type sectors[DEFAULT_ORDER];
	struct node * parent;
	bool is_leaf;
	uint32_t num_keys;
	struct node * next; // Used for queue.
	sect_type sector;
} node;



// TYPES.

/* Type representing the record
 * to which a given key refers.
 * In a real B+ tree system, the
 * record would hold data (in a database)
 * or a file (in an operating system)
 * or some other information.
 * Users can rewrite this part of the code
 * to change the type and content
 * of the value field.
 */


/* This is the record where all the data is stored,
 * the record is composed by a key (that is not primary so it can have multiple data associated)
 * all the values of the record, the parent node (that can also be a record node because of multiple keys)
 * the sector of the child (zero if there is no child) and a pointer to it that is equal to NULL 
 * if the  shild exists and is not in the cache or the child doesn't exists, otherwise point to the child.
 * the parent pointer is used only when the tree is in cache for simplify the insertion, because the parent node is surely already in the cache.
 * the sector is the sector used for that node.
*/
typedef struct record {
	key_type key;
	fields_type * fields;
	node * parent;
	struct record * next;
	sect_type next_sect;
	sect_type sector;
} record;

typedef struct pair_queue {
	node * node_pointer;
	int height;
} pair_queue;

#endif
