#include "cli_socket.h"
#include "utils_tree.h"
#include "header_cli.h"
#include "cli_server.h"
#include "find_and_print.h"
#include <map>
#include <list>	
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <cctype>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>
#include <iostream>


void *retrieveDataFake(int socketfd, sect_type sector, bool is_record, node * root, int root_index){
	std::list<sect_type> nodes_to_retrieve;
	std::list<sect_type>::const_iterator it_list;
	int list_to_put, old = 0;
	sect_type last_sect_used;
	std::set<sect_type> used_set_local;
	std::set<sect_type>::iterator iter;
	unsigned char * retrieved;
	int i, flag = 0, j = 0;
	uint32_t len;
	used_set_local.clear();
	std::map<sect_type, void*>::const_iterator it;
	it = nodes_in_memory.find(sector);
	if (it!=nodes_in_memory.end()) {
		used_set_local.insert(sector);
		return nodes_in_memory[sector];
	}
	nodes_to_retrieve.push_back(root_index);
	used_set_local.insert(root_index);
	int num = (root->num_keys-1)/2;
	while (j < num)
	{
		list_to_put = (rand() % root->num_keys);
		iter = used_set_local.find(list_to_put);
		if(iter == used_set_local.end())
		{
			nodes_to_retrieve.push_back(list_to_put);
			used_set_local.insert(list_to_put);
			j++;
		}
	}
	nodes_to_retrieve.sort();
	i = 0;
	for(it_list=nodes_to_retrieve.begin();it_list!=nodes_to_retrieve.end(); ++it_list) {
		i++;
		//sec is the pointer to the sector retrieved
		it = nodes_in_memory.find(root->sectors[*it_list]);
		if (it == nodes_in_memory.end()) {
			len = retrieveData(socketfd, root->sectors[*it_list], (void **)&retrieved);
			if(is_record) {
				record * r, *r_prev;
				uint32_t rec_next;
				r = (record *) deserialize(retrieved, 1, len);
				root->pointers[*it_list] = r;
				r->parent = root;
				rec_next = r->next_sect;
				r_prev = r;
				used_set.insert(r->sector);
				nodes_in_memory[r->sector] = r;
				//std:: cout << "i record sono questi " << std::endl;
				//std::cout << "rec next " << rec_next <<std::endl;
				//print_record(r);
				while (rec_next > 0) {
					len = retrieveData(socketfd, rec_next, (void **)&retrieved);
					r = (record *) deserialize(retrieved, 1, len);
					r_prev->next = r;
					r->parent = root;
					rec_next = r->next_sect;
					r_prev = r;
					used_set.insert(r->sector);
					nodes_in_memory[r->sector] = r;
					//print_record(r);
				}
				//std::cout << "finiti di recuperare" << std::endl;
			}
			else {
				node * n;
				n = (node *) deserialize(retrieved, 0, len);
				root->pointers[*it_list] = n;
				n->parent = root;
				used_set.insert(root->sectors[*it_list]);
				nodes_in_memory[root->sectors[*it_list]] = root->pointers[*it_list];

			}
//			used_set.insert(root->sectors[*it_list]);
//			nodes_in_memory[root->sectors[*it_list]] = root->pointers[*it_list];
		}		
	}
	return nodes_in_memory[sector];
}

int send_tree(node * root) {
	sect_type sector, last;
    srand(time(NULL));
    	nodes_in_memory.clear();
	int len_end;
	node * n, *aus;
	record * rec;
	unsigned char * ser;
	std::set<sect_type>::iterator iter;
	std::list<node *> remaining;
	int rnd, size;
	size = used_set.size();
	rnd = (rand() % size);
	iter = used_set.begin();
	advance(iter, rnd);
	root->sector = *iter;
	retr->root_sect = *iter;
	used_set.erase(*iter);
	remaining.push_back(root);
	while(remaining.size() > 0) {
		n = remaining.front();
		remaining.pop_front();
		if(!n->is_leaf) {
			for(int i = 0; i <= n->num_keys;i++) {
				if(n->pointers[i] != NULL) {
					remaining.push_back((node *)n->pointers[i]);
					rnd = (rand() % used_set.size());
					iter = used_set.begin();
					advance(iter, rnd);
					aus = (node *)n->pointers[i];
					aus->sector = *iter;
					n->sectors[i] = *iter;
					used_set.erase (*iter);
				}
			}
			last = n->sector;
			ser = serialize(n,0, &len_end);
			sendData(socketfd, ser, last, len_end);	
			delete[] ser;		
		}
		else {
			for(int i = 0; i < n->num_keys;i++) {
				if(n->pointers[i] != NULL) {
					record * p_rec, *p_rec_old;
					p_rec_old = NULL;
					p_rec = (record *)n->pointers[i];
					//this is wrong
					while (p_rec != NULL) {
						rnd = rand() % used_set.size();
						iter = used_set.begin();
						advance(iter, rnd);
						//std::cout << "sending " << *iter <<std::endl;
						p_rec->sector = *iter;
						if(p_rec == n->pointers[i]) {
							//std::cout << "p_rec == n->pointers[i]" << std::endl;
							n->sectors[i] = *iter;
						}
						else {
							p_rec_old->next_sect = *iter;
							//std::cout << "p_rec != n->pointers[i]" << std::endl;
						}
						used_set.erase(*iter);
						if(p_rec_old != NULL){
							//std::cout << "serializing" << std::endl;
							last = p_rec_old->sector;
							print_record(p_rec_old);
							ser = serialize(p_rec_old,1, &len_end);
							n->pointers[i] = NULL;
							sendData(socketfd, ser, last, len_end);
							delete[] ser;
						}
						p_rec_old = p_rec;
						p_rec = p_rec_old->next;
						//std::cout << "done sending " << *iter <<std::endl;
					}
					if(p_rec_old != NULL){
						//std::cout << "serializing" << std::endl;
						last = p_rec_old->sector;
						print_record(p_rec_old);					
						ser = serialize(p_rec_old,1, &len_end);
						n->pointers[i] = NULL;
						sendData(socketfd, ser, last, len_end);
						delete[] ser;
					}
				}
			}
			last = n->sector;
			ser = serialize(n,0, &len_end);
			sendData(socketfd, ser, last, len_end);
			delete[] ser;		
		}
	}
	serialize_info();
	print_nodes_and_sectors();
}


void * retrieveRoot(void) {
	node * root;
	unsigned char * n;
	uint32_t len;
	deserialize_info();
	len = retrieveData(socketfd, retr->root_sect, (void **)&n);
	root = (node *)deserialize(n, 0, len);
	used_set.insert(root->sector);
	nodes_in_memory[root->sector] = root;
	return root;
}
