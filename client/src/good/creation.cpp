#include "creation.h"
/* Creates a new record to hold the value
 * to which a key refers.
 */
record * make_record(key_type key, fields_type * fields) {
	record * new_record = new record;
	if (new_record == NULL) {
		perror("Record creation.");
		exit(EXIT_FAILURE);
	}
	else {
		//here you have to copy and add all fields in the record
		//new_record->value = value;
		new_record->key = key;
		new_record->parent = NULL;
		new_record->next = NULL;
		new_record->next_sect = 0;
//	if (sect_not_used.empty()) {
		retr->last_sect_used++;
		new_record->sector = retr->last_sect_used;
		std::cout << "key " << key << " sector " << new_record->sector << "\n";
//	}
//	else
//		new_record->sector = sect_not_used.pop_front();
		new_record->fields = fields;
		nodes_in_memory[new_record->sector] = new_record;
		used_set.insert(new_record->sector);
		//nodes_used[new_record->sector] = new_record;
		return new_record;
	}
}


/* Creates a new general node, which can be adapted
 * to serve as either a leaf or an internal node.
 */
node * make_node( void ) {
	node * new_node;
	new_node = new node;
	if (new_node == NULL) {
		perror("Node creation.");
		exit(EXIT_FAILURE);
	}
	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	//new_node->sector_next = 0;
//	if (sect_not_used.empty()) {
		retr->last_sect_used++;
		new_node->sector = retr->last_sect_used;
		std::cout << " sector node " << new_node->sector << "\n";

//	}
//	else
//		new_node->sector = sect_not_used.pop_front();
	nodes_in_memory[new_node->sector] = new_node;
	used_set.insert(new_node->sector);
	//nodes_used[new_node->sector] = new_node;
	return new_node;
}

/* Creates a new leaf by creating a node
 * and then adapting it appropriately.
 */
node * make_leaf( void ) {
	node * leaf = make_node();
	leaf->is_leaf = true;
	return leaf;
}

