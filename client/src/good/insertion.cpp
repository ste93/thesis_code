#include "insertion.h"

/* Helper function used in insert_into_parent
 * to find the index of the parent's pointer to 
 * the node to the left of the key to be inserted.
 */
 //here I have both parent and left node in memory
int get_left_index(node * parent, node * left) {

	int left_index = 0;
	while (left_index <= parent->num_keys && parent->sectors[left_index] != left->sector)
		left_index++;
	return left_index;
}

/* Inserts a new pointer to a record and its corresponding
 * key into a leaf.
 * Returns the altered leaf.
 */
node * insert_into_leaf( node * leaf, key_type key, record * pointer ) {

	int i, insertion_point;

	insertion_point = 0;
	while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
		insertion_point++;
		
	for (i = leaf->num_keys; i > insertion_point; i--) {
		leaf->keys[ i] = leaf->keys[i - 1];
		leaf->pointers[i] = leaf->pointers[i - 1];
		leaf->sectors[i] = leaf->sectors[i - 1];
	}
	leaf->keys[insertion_point] = key;
	leaf->pointers[insertion_point] = pointer;
	leaf->sectors[insertion_point] = pointer->sector;
	leaf->num_keys++;
	return leaf;
}


/* Inserts a new key and pointer
 * to a new record into a leaf so as to exceed
 * the tree's order, causing the leaf to be split
 * in half.
 */
node * insert_into_leaf_after_splitting(node * root, node * leaf, key_type key, record * pointer) {

	node * new_leaf;
	key_type temp_keys[DEFAULT_ORDER];
	void * temp_pointers[DEFAULT_ORDER];
	sect_type temp_sect[DEFAULT_ORDER];
	int insertion_index, split, i, j;
	key_type new_key;

	new_leaf = make_leaf();

	insertion_index = 0;
	while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
		insertion_index++;

	for (i = 0, j = 0; i < leaf->num_keys; i++, j++) {
		if (j == insertion_index) j++;
		temp_keys[j] = leaf->keys[i];
		temp_pointers[j] = leaf->pointers[i];
		temp_sect[j] = leaf->sectors[i];
	}
	temp_keys[insertion_index] = key;
	temp_pointers[insertion_index] = pointer;
	std::cout << "insertion into leaf after splitting pointer sector: " << pointer->sector << "\n";
	temp_sect[insertion_index] = pointer->sector;

	leaf->num_keys = 0;

	split = cut(order - 1);

	for (i = 0; i < split; i++) {
		leaf->pointers[i] = temp_pointers[i];
		leaf->keys[i] = temp_keys[i];			
		leaf->sectors[i] = temp_sect[i];
		leaf->num_keys++;
	}
	for (i = split, j = 0; i < order; i++, j++) {
		new_leaf->pointers[j] = temp_pointers[i];
		new_leaf->keys[j] = temp_keys[i];
		new_leaf->sectors[j] = temp_sect[i];
		new_leaf->num_keys++;
	}
	//new_leaf->pointers[order - 1] = leaf->pointers[order - 1];
	//leaf->pointers[order - 1] = new_leaf;

	for (i = leaf->num_keys; i < order; i++)
		leaf->pointers[i] = NULL;
	for (i = new_leaf->num_keys; i < order; i++)
		new_leaf->pointers[i] = NULL;

	new_leaf->parent = leaf->parent;
//	new_leaf->parent_sect = leaf->parent_sect;
	new_key = new_leaf->keys[0];

	return insert_into_parent(root, leaf, new_key, new_leaf);
}


/* Inserts a new key and pointer to a node
 * into a node into which these can fit
 * without violating the B+ tree properties.
 */
node * insert_into_node(node * root, node * n, int left_index, key_type key, node * right) {
	int i;
	for (i = n->num_keys; i > left_index; i--) {
		n->pointers[i + 1] = n->pointers[i];
		n->keys[i] = n->keys[i - 1];
		n->sectors[i + 1] = n->sectors[i];
	}
	n->pointers[left_index + 1] = right;
	n->keys[left_index] = key;
	n->sectors[left_index + 1] = right->sector;
	n->num_keys++;
	return root;
}


/* Inserts a new key and pointer to a node
 * into a node, causing the node's size to exceed
 * the order, and causing the node to split into two.
 */
node * insert_into_node_after_splitting(node * root, node * old_node, int left_index, key_type key, node * right) {

	int i, j, split;
	key_type k_prime;
	node * new_node, * child;
	key_type temp_keys[DEFAULT_ORDER + 1];
	sect_type temp_sect[DEFAULT_ORDER + 1];
	void * temp_pointers[DEFAULT_ORDER + 1];

	std::cout << "insert_into_node_after_splitting " << key << "\n" ;
	/* First create a temporary set of keys and pointers
	 * to hold everything in order, including
	 * the new key and pointer, inserted in their
	 * correct places. 
	 * Then create a new node and copy half of the 
	 * keys and pointers to the old node and
	 * the other half to the new.
	 */
	std::cout << "\n settori dello split"; 
	for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) {
		if (j == left_index + 1) j++;
		temp_pointers[j] = old_node->pointers[i];
		temp_sect[j] = old_node->sectors[i];
		std::cout << old_node->sectors[i] << ";";
	}
	std::cout << "\n";

	for (i = 0, j = 0; i < old_node->num_keys; i++, j++) {
		if (j == left_index) j++;
		temp_keys[j] = old_node->keys[i];
	}

	temp_pointers[left_index + 1] = right;
	temp_sect[left_index + 1] = right->sector;
	temp_keys[left_index] = key;

	/* Create the new node and copy
	 * half the keys and pointers to the
	 * old and half to the new.
	 */  
	split = cut(order);
	new_node = make_node();
	old_node->num_keys = 0;
	for (i = 0; i < split - 1; i++) {
		old_node->pointers[i] = temp_pointers[i];
		old_node->sectors[i] = temp_sect[i];
		old_node->keys[i] = temp_keys[i]; 
		old_node->num_keys++;
	}
	old_node->pointers[i] = temp_pointers[i];
	old_node->sectors[i] = temp_sect[i];
	k_prime = temp_keys[split - 1];
	i++;
	for (j = 0; i < order; i++, j++) {
		new_node->pointers[j] = temp_pointers[i];
		new_node->sectors[j] = temp_sect[i];
		new_node->keys[j] = temp_keys[i];
		std::cout << " sect " << temp_sect[i] << " keys " << temp_keys[i] << " i "<<i << " ; ";
		new_node->num_keys++;
	}
	std::cout <<"\n i "<< temp_sect[i]<<"\n";
	new_node->pointers[j] = temp_pointers[i];
	new_node->sectors[j] = temp_sect[i];
	new_node->parent = old_node->parent;
	for (i = 0; i <= new_node->num_keys; i++) {
		child = (node *)new_node->pointers[i];
		child->parent = new_node;
	}

	/* Insert a new key into the parent of the two
	 * nodes resulting from the split, with
	 * the old node to the left and the new to the right.
	 */

	return insert_into_parent(root, old_node, k_prime, new_node);
}



/* Inserts a new node (leaf or internal node) into the B+ tree.
 * Returns the root of the tree after insertion.
 */
node * insert_into_parent(node * root, node * left, key_type key, node * right) {

	int left_index;
	node * parent;
	std::cout << "insert into parent: " << key << " " << left->sector << " " << right->sector << " " << root->sector << " " << left->parent << "\n";
	parent = left->parent;

	/* Case: new root. */

	if (parent == NULL)
		return insert_into_new_root(left, key, right);

	/* Case: leaf or node. (Remainder of
	 * function body.)  
	 */

	/* Find the parent's pointer to the left 
	 * node.
	 */

	left_index = get_left_index(parent, left);


	/* Simple case: the new key fits into the node. 
	 */

	if (parent->num_keys < order - 1)
		return insert_into_node(root, parent, left_index, key, right);

	/* Harder case:  split a node in order 
	 * to preserve the B+ tree properties.
	 */

	return insert_into_node_after_splitting(root, parent, left_index, key, right);
}


/* Creates a new root for two subtrees
 * and inserts the appropriate key into
 * the new root.
 */
node * insert_into_new_root(node * left, key_type key, node * right) {

	node * root = make_node();
	root->keys[0] = key;
	root->pointers[0] = left;
	root->sectors[0] = left->sector;
	root->pointers[1] = right;
	root->sectors[1] = right->sector;
	root->num_keys++;
	root->parent = NULL;
	left->parent = root;
	right->parent = root;
	return root;
}



/* First insertion:
 * start a new tree.
 */
node * start_new_tree(key_type key, record * pointer) {
	node * root = make_leaf();
	root->keys[0] = key;
	root->pointers[0] = pointer;
	root->sectors[0] = pointer->sector;
	root->pointers[order - 1] = NULL;
	root->parent = NULL;
	root->num_keys++;
	return root;
}



/* Master insertion function.
 * Inserts a key and an associated value into
 * the B+ tree, causing the tree to be adjusted
 * however necessary to maintain the B+ tree
 * properties.
 */ 
node * insert( node * root, key_type key, fields_type * fields ) {

	record * pointer, *pp, *orig;
	node * leaf;
	int i;
	std::cout << "inserting : " << key << " ; ";
	for (i = 0; i < retr->num_fields; i++)
		std::cout << fields[i] << " ";
	std::cout << "\n";
	std::cout << root;
	if (root != NULL) {
	//this one is for the duplicates
		if ((pp = find(root, key, false)) != NULL){
			std::cout << "found " << key;
/*			orig = pp;
			while (pp->next_sect != 0){//here in the queue the different values for a single key are not in order, can be updated
				//here I have to manage the pointers
				pp->next = (record *)retrieveDataFake(socketfd, pp->next_sect, true);
				pp->next->parent = leaf;
	//			if (pp->key == value)
	//				return NULL;
				pp = pp->next;
			}
	//		if (pp->key == value)
	//			return NULL;
			pp->next = make_record(key, fields);
			//pp->next->prev = pp;
			pp->next->parent = pp->parent;
			pp->next_sect = pp->next->sector;
			pp->next->next_sect = 0;
			*/
			return root;
		}
	}
	/* Create a new record for the value.	 */
	pointer = make_record(key, fields);
	//std::cout << "record  " << pointer->sector << std::endl;
	/* Case: the tree does not exist yet. Start a new tree. */
	if (root == NULL) 
		return start_new_tree(key, pointer);

	/* Case: the tree already exists. (Rest of function body.) */
	leaf = find_leaf(root, key, false);
	/* Case: leaf has room for key and pointer. */
	if (leaf->num_keys < order - 1) {
		//std::cout << "key into leaf " << key << "\n";
		leaf = insert_into_leaf(leaf, key, pointer);
		return root;
	}
	/* Case:  leaf must be split. */
	return insert_into_leaf_after_splitting(root, leaf, key, pointer);
}
