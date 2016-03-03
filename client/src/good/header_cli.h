#ifndef HEADERS_CLIENT
#define HEADERS_CLIENT
#include "const_cli.h"
// GLOBALS.

/* The order determines the maximum and minimum
 * number of entries (keys and pointers) in any
 * node.  Every node has at most order - 1 keys and
 * at least (roughly speaking) half that number.
 * Every leaf has as many pointers to data as keys,
 * and every internal node has one more pointer
 * to a subtree than the number of keys.
 * This global variable is initialized to the
 * default value.
 */

/* The queue is used to print the tree in
 * level order, starting from the root
 * printing each entire rank on a separate
 * line, finishing with the leaves.
 */
extern node * queue;

extern int order;

/* The user can toggle on and off the "verbose"
 * property, which causes the pointer addresses
 * to be printed out in hexadecimal notation
 * next to their corresponding keys.
 */
extern bool verbose_output;

//this is the socket file descriptor for the connection between client and server
extern int socketfd;

//this variable contains the informations stored on the first sector of the hard drive (as above)
extern info * retr;


/* this contains all the sector of the nodes in memory mapped with 
 * the relative sector, this allow the client to see if the node is already in cache or if the node 
 * is to be retrieved.
 */
extern std::map<sect_type, void*> nodes_in_memory;
extern std::map<sect_type, void*> nodes_used;
extern std::set<sect_type> used_set;
extern std::map<sect_type, sect_type> old_new_mapping;
extern std::list<sect_type> sect_not_used;
#endif
