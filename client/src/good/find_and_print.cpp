#include "find_and_print.h"

/* Prints the B+ tree in the command
 * line in level (rank) order, with the 
 * keys in each node and the '|' symbol
 * to separate nodes.
 * With the verbose_output flag set.
 * the values of the pointers corresponding
 * to the keys also appear next to their respective
 * keys, in hexadecimal notation.
 */
 //this is not necessary because needs all te tree, but instead I can print the local tree
 //but I can manage this later
/*void print_tree( node * root ) {
	node * n = NULL;
	int i = 0;
	int rank = 0;
	int new_rank = 0;

	if (root == NULL) {
		printf("Empty tree.\n");
		return;
	}
	queue = NULL;
	enqueue(root);
	while(queue != NULL) {
		n = dequeue();
		//this is not working
		//this is the new branch of the tree, must be managed correctly
		if (n->parent != NULL && n->parent->pointers[0] != NULL){
			if (n == n->parent->pointers[0]) {
				new_rank = path_to_root( root, n );
				if (new_rank != rank) {
					rank = new_rank;
					printf("\n");
				}
			}
		}
		if (verbose_output) 
			printf("(%lx)", (unsigned long)n);
		for (i = 0; i < n->num_keys; i++) {
			if (verbose_output)
				printf("%lx ", (unsigned long)n->pointers[i]);
			printf("%d ", n->keys[i]);
		}
		if (!n->is_leaf)
			for (i = 0; i <= n->num_keys; i++) 
				if (n->pointers[i] != NULL) 
					enqueue((node *)n->pointers[i]);
		if (verbose_output) {
			if (n->is_leaf) 
				printf("%lx ", (unsigned long)n->pointers[order - 1]);
			else
				printf("%lx ", (unsigned long)n->pointers[n->num_keys]);
		}
		printf("| ");
	}
	printf("\n");
}
*/
/*
void print_tree (node * root) {
	if (root != NULL) {
	}
}
		
/* Finds the record under a given key and prints an
 * appropriate message to stdout.
 */
 
void print_nodes_and_sectors() {
	int j = 0;
	for (std::map<sect_type, void*>::iterator it=nodes_in_memory.begin(); it!=nodes_in_memory.end(); ++it) { 
		std::cout << it->first << " => " << it->second << '\n';
	}
}

void find_and_print(node * root, key_type key, bool verbose) {
	record * r = find(root, key, verbose);
	//if (r == NULL)
		//printf("Record not found under key %d.\n", key);
	//else {
		//std::cout << "record";
		//print_record(r);
	//}
}
/*
void print_tree (node * root) {
	std::queue<pair_queue> q;
	std::queue<record *> r;
	
}
*//*
void print_tree_sec(node * root, int level, std::queue<pair_queue> * q, std::queue<record *> r) {
	int i;
	int flag = 0;
	std::cout << root->key << " " << root->;
 	for(i = 0; i < root->num_keys; i++) {
		if(root->pointers[i] != NULL) {
			if(root->is_leaf) {
				r.push(<(record *)root->pointers[i]>);
			}
			else {
				q.push(<(node *)root->pointers[i], level>);
			}
		}
	}
	if (!q.empty()) {
		
}
*/
void print_node(node * ptr) {
	int i;
	if (ptr->is_leaf) {
		std::cout << ptr->key << " ";
		for(i = 0; i < ptr->num_keys;i++) {
			std::cout << ptr->keys[i] << " ";
			print_record((record *)ptr->pointers[i]);
			std::cout << " ; ";
		}
		std::cout << ptr->sector << std::endl;
	}
}

void print_record(record * ptr) {
	int i;
	std::cout << ptr << std::endl;
	std::cout << " num fields " << retr->num_fields << std::endl;
/*	while (ptr->next != NULL) {
		std::cout << ptr->key << " : ";
		
		for(i = 0; i < retr->num_fields; i++) {
			std::cout << ptr->fields[i] << " , ";
		}
		std::cout << ptr->sector;
		std::cout << std::endl;
		ptr = ptr->next;
	}*/
	std::cout << ptr->key << " : ";
	for(i = 0; i < retr->num_fields; i++) {
		std::cout << ptr->fields[i] << " , ";
	}
	std::cout << std::endl << ptr->next_sect << " " << ptr->next << std::endl;
	std::cout << " sector " << ptr->sector;
	std::cout << std::endl;
}

/*

void print_all() {
	std::map<sect_type, void*>::const_iterator iter;
	for (iter = nodes_used.begin(); iter != nodes_used.end(); iter++)
	{
		std::cout << "Key: " << iter->first << "Values: ";
		std::cout << iter->second->
	}
}

/* Finds and prints the keys, pointers, and values within a range
 * of keys between key_start and key_end, including both bounds.
 */

void find_and_print_range( node * root, key_type key_start, key_type key_end, bool verbose) {
	int i;
	std::list<record*> *returned_rec;
	std::list<record*>::const_iterator rec_it;
	returned_rec = new std::list<record*>;
	int num_found = 0;
	record * ptr_to_rec;
	std::cout << "find print_range()";
	find_range( root, key_start, key_end, verbose, returned_rec);
	//ptr_to_rec = returned_rec;
	std::cout << "found: " << std::endl;
	if ((*returned_rec).empty())
		printf("None found.\n");
	else {
		for(rec_it=(*returned_rec).begin();rec_it!=(*returned_rec).end(); ++rec_it) {
			//print_rec(ptr_to_rec);
			print_record(*rec_it);
		}
	}
}

/* Finds keys and their pointers, if present, in the range specified
 * by key_start and key_end, inclusive.  Places these in the arrays
 * returned_keys and returned_pointers, and returns the number of
 * entries found.
 */
//the root is the root of the subtree
//is guaranteed the order because the search is a deep first search, also is guaranteed completeness because the tree is finite
//at the end in returned rec there is a list with all the records in that range
void find_range(node * root, key_type key_start, key_type key_end, bool verbose, std::list<record*> * returned_rec) {
	int i, flag = 0;
	record * ptr_to_rec, * ptr_tmp;
	node * ptr, * ptr_main;
	std::list<node *> list_search;
	std::list<node *>::const_iterator it;
	//std::cout << "find_range() key start " << key_start << " key end " << key_end << " num keys " << root->num_keys << " : " << std::endl;
	list_search.push_back(root);
	while(!list_search.empty()) {
		//std::cout << " in_list ";
		for(it=list_search.begin();it!=list_search.end(); ++it) {
			//print_rec(ptr_to_rec);
			//std::cout << (*it)->sector<< " ; ";
		}
		//std::cout << std::endl;
		ptr_main = list_search.front();
		list_search.pop_front();
		for(i=0;i<ptr_main->num_keys;i++) {
			//std::cout << " round for " << i << " " << ptr_main->keys[i] << " " << key_start << " ; " << std::endl; 
			if (!ptr_main->is_leaf) {
				//std::cout << " ! leaf " << std::endl;
				//ptr = new (void *);
				if(i==0 && ptr_main->keys[i] >= key_start){
					std::cout << "case 1" << std::endl;
					ptr = (node *)retrieveDataFake(socketfd, ptr_main->sectors[i], false, ptr_main, i);
					//ptr_main->pointers[i] = ptr;
					//ptr->parent = ptr_main;
					list_search.push_back(ptr);
				}
				//std::cout << "after first if " << std::endl;
				if(i!=0 && ((ptr_main->keys[i] > key_start) || (ptr_main->keys[i-1] <= key_end))) {
					std::cout << "case 2 " <<  std::endl;
					ptr = (node *)retrieveDataFake(socketfd, ptr_main->sectors[i], false, ptr_main, i);
					//ptr_main->pointers[i] = ptr;
					//ptr->parent = ptr_main;
					list_search.push_back(ptr);				
				}
				//std::cout << "after second if " << i << " " << ptr_main->num_keys <<" " << ptr_main->keys[i] << " " << ptr_main->sectors[i] << " " << key_start << " " << key_end << " ; " << std::endl; 
				if (i==ptr_main->num_keys-1 && ptr_main->keys[i] < key_end) {
					std::cout << "case 3" << std::endl;
					ptr = (node *)retrieveDataFake(socketfd, ptr_main->sectors[i + 1], false, ptr_main, i+1);
					//std::cout << "after case 3" <<std::endl;
					//ptr_main->pointers[i+1] = ptr;
					//ptr->parent = ptr_main;
					//std::cout << "before list search" << std::endl;
					list_search.push_back(ptr);
					//std::cout << "after list search" << std::endl;
				}
			}
			else {
				if(ptr_main->keys[i]<=key_end && ptr_main->keys[i]>=key_start){
				//here I have to manage the pointers
					//std:: cout << "second case" << std::endl;
					ptr_to_rec = (record *)retrieveDataFake(socketfd, ptr_main->sectors[i], true, ptr_main, i);
					//std::cout << "record retrieved " << std::endl;
					//ptr_main->pointers[i] = ptr_to_rec;
					//ptr_to_rec->next = NULL;
					//ptr_to_rec->parent = ptr_main;
					//std::cout << "pushing record" << std::endl;
					(*returned_rec).push_back(ptr_to_rec);
					while(ptr_to_rec->next_sect != 0) {
						//here I have to manage the pointers
						std::cout << "range " << std::endl;
						std::cout << ptr_to_rec->next << " " << ptr_to_rec << " " << ptr_to_rec->next_sect << std::endl;
						ptr_to_rec = ptr_to_rec->next;
						(*returned_rec).push_back(ptr_to_rec);
					}
					//here I have to manage the pointers
	/*				ptr_to_rec = (record *)retrieveDataFake(socketfd, ptr_main->sectors[i], true);
					ptr_to_rec->next = returned_rec;
					returned_rec = ptr_to_rec;
					ptr_to_rec->parent = ptr_main;
					num_found++;
					* */
				}
			}
		}
	}
}

/*	int i, num_found;
	num_found = 0;
	node * n = find_leaf( root, key_start, verbose );
	if (n == NULL) return 0;
	for (i = 0; i < n->num_keys && n->keys[i] < key_start; i++) ;
	if (i == n->num_keys) return 0;
	while (n != NULL) {
		for ( ; i < n->num_keys && n->keys[i] <= key_end; i++) {
			returned_keys[num_found] = n->keys[i];
			returned_pointers[num_found] = n->pointers[i];
			num_found++;
		}
		n = n->pointers[order - 1];
		i = 0;
	}
	return num_found;
}
*/


/* Traces the path from the root to a leaf, searching
 * by key.  Displays information about the path
 * if the verbose flag is set.
 * Returns the leaf containing the given key.
 */
 //this function is useful
node * find_leaf( node * root, key_type key, bool verbose ) {
	int i = 0;
	node * c = root;
	node * pp;
	if (c == NULL) {
		if (verbose) 
			printf("Empty tree.\n");
		return c;
	}
	std::cout << "find leaf\n";
	while (!c->is_leaf) {		
		std::cout << "ciao" << std::endl;
		/*
		if (verbose) {
			printf("[");
			for (i = 0; i < c->num_keys - 1; i++)
				printf("%d ", c->keys[i]);
			printf("%d] ", c->keys[i]);
		}
		*/
		i = 0;
		//std::cout << key << " " << c->keys[i];
		while (i < c->num_keys) {
			std::cout << key << " alalalala " << c->keys[i] << "\n";
			if (key >= c->keys[i]) 
				i++;
			else 
				break;
		}
		std::cout << "i " << i << std::endl;
		/*
		if (verbose)
			printf("%d ->\n", i);
		*/
		std::cout << "sectors "<< c->sectors[i] << "\n";
		//here I have to manage the pointers
		pp = c;
		c = (node *)retrieveDataFake(socketfd, c->sectors[i], false, c, i);
		pp->pointers[i] = c;
	}
	/*std::cout << "key " << c->keys[0] << " "  << c->keys[1] << "\n";  
	for (i=0;i<c->num_keys;i++) {
		std::cout << c->keys[i] << " ";
	}
	/*
	if (verbose) {
		printf("Leaf [");
		for (i = 0; i < c->num_keys - 1; i++)
			printf("%d ", c->keys[i]);
		printf("%d] ->\n", c->keys[i]);
	}
	*/
	return c;
}


/* Finds and returns the record to which
 * a key refers.
 */
 //this function find a leaf with the specified key if it exists and returns the pointer to the record found, returns NULL otherwise
record * find( node * root, key_type key, bool verbose ) {
	int i = 0;
	record * pp, *first;
	std::cout << key << "\n";
	uint32_t rec_next;
	node * c = find_leaf( root, key, verbose );
	std::cout << "found leaf\n";
	if (c == NULL) return NULL;
	for (i = 0; i < c->num_keys; i++) {
		std::cout << "key " <<  c->keys[i] << " " << c->sectors[i] << "\n";
		if (c->keys[i] == key) break;
	}
	//std::cout << "asdadsada "<< i << " " << c->num_keys << "\n";
	if (i == c->num_keys) 
		return NULL;
	else {
		/* this points to the first record with that key */
		//std::cout << "asfdsadf" << c->sectors[i] << "\n";
		//here I have to manage the pointers
		//rec_next = c->sectors[i];
		pp = (record *)retrieveDataFake(socketfd, c->sectors[i], true, c, i);
		print_record(pp);
		first = pp;
		std::cout << pp->next_sect << std::endl;
		std::cout << pp->next << std::endl;
		while(pp->next_sect > 0){
			//std::cout << "ciao" << std::endl;
			pp = pp->next;
			print_record(pp);
			//std::cout << pp->next_sect;
		}
		//pp->parent = c;
		//std::cout << pp << std::endl;
		//print_record(pp);
		return first;
	}
}
