#include "cli_socket.h"
//#include "bpt.cpp"
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
	//std::cout << "retrieveDataFake\n";
	used_set_local.clear();
	std::map<sect_type, void*>::const_iterator it;
	it = nodes_in_memory.find(sector);
	if (it!=nodes_in_memory.end()) {
		//std::cout << "found" << std::endl;
		//nodes_used[sector] = nodes_in_memory[sector];
		used_set_local.insert(sector);
		return nodes_in_memory[sector];
	}
	nodes_to_retrieve.push_back(root_index);
	used_set_local.insert(root_index);
	//last_sect_used = retr->last_sect_used;
	int num = (root->num_keys-1)/2;
	std::cout << " num " << num << std::endl;
	std::cout << root->sector << std::endl;
	while (j < num)
	{
		list_to_put = (rand() % root->num_keys);
		//std::cout << list_to_put << std::endl;
		iter = used_set_local.find(list_to_put);
		//std::cout << *iter;
		std::cout << "ciaociao" << std::endl;
		if(iter == used_set_local.end())
		{
			nodes_to_retrieve.push_back(list_to_put);
			used_set_local.insert(list_to_put);
			for(iter=used_set_local.begin(); iter!=used_set_local.end();++iter) 
				std::cout << *iter << " " << root->sectors[*iter] << " ; ";
			j++;
		}
	}
	nodes_to_retrieve.sort();
	//std::cout << "retrieved" <<std::endl;
	i = 0;
	for(it_list=nodes_to_retrieve.begin();it_list!=nodes_to_retrieve.end(); ++it_list) {
		i++;
		//std::cout << i << std::endl;
		//sec is the pointer to the sector retrieved
		std::cout << "retrieving " << root->sectors[*it_list] << "from" << *it_list <<  std::endl;
		it = nodes_in_memory.find(root->sectors[*it_list]);
		//std::cout << sector << "sect" << std::endl;
		if (it == nodes_in_memory.end()) {
			//std::cout << "retrieving" << std::endl;
			len = retrieveData(socketfd, root->sectors[*it_list], (void **)&retrieved);
			if(is_record) {
				record * r, *r_prev;
				uint32_t rec_next;
				r = (record *) deserialize(retrieved, 1, len);
				root->pointers[*it_list] = r;
				r->parent = root;
				rec_next = r->next_sect;
				r_prev = r;
				while (rec_next > 0) {
					len = retrieveData(socketfd, rec_next, (void **)&retrieved);
					r = (record *) deserialize(retrieved, 1, len);
					r_prev->next = r;
					r->parent = root;
					rec_next = r->next_sect;
					used_set.insert(rec_next);
					nodes_in_memory[rec_next] = r;
					std::cout << " asdfa " << std::endl;
					print_record(r);
				}
				std::cout << "sector " << r->sector<< std::endl;
			}
			else {
				node * n;
				n = (node *) deserialize(retrieved, 0, len);
				root->pointers[*it_list] = n;
				n->parent = root;
				//std::cout << "sector " << n->sector<< std::endl;
			}
			//std::cout << "retrieved correctly " << std::endl;
			//std::cout << "after if" << std::endl;
			//std::cout << " asdfa " << *it_list << std::endl;
			//for(iter=used_set.begin(); iter!=used_set.end();++iter) 
			//	std::cout << *iter;
			//std::cout << "this is the set" << std::endl;
			used_set.insert(root->sectors[*it_list]);
			//std::cout << "after if " << root->sectors[*it_list] <<  std::endl;
			nodes_in_memory[root->sectors[*it_list]] = root->pointers[*it_list];
			
		}		
		//std::cout << "retrieved " << root->sectors[*it_list] << std::endl;
	}
	std::cout << "exiting" << std::endl;
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
	//std::cout << used_set.size()<< std::endl;
	rnd = (rand() % size);
	//std::cout << "done random" << std::endl;
	iter = used_set.begin();
	advance(iter, rnd);
	root->sector = *iter;
	retr->root_sect = *iter;
	used_set.erase(*iter);
	remaining.push_back(root);
	//std::cout << "before root" << std::endl;
	while(remaining.size() > 0) {
		n = remaining.front();
		remaining.pop_front();
		if(!n->is_leaf) {
			for(int i = 0; i <= n->num_keys;i++) {
				std::cout << "sending data" << std::endl;
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
			//std::cout << "serializing parent" << std::endl;
			last = n->sector;
			ser = serialize(n,0, &len_end);
			sendData(socketfd, ser, last, len_end);	
			delete[] ser;		
		}
		else {
			for(int i = 0; i < n->num_keys;i++) {
				if(n->pointers[i] != NULL) {
					//std::list<record *> to_ser;
					record * p_rec, *p_rec_old;
					p_rec_old = NULL;
					p_rec = (record *)n->pointers[i];
					//this is wrong
					while (p_rec != NULL) {
						rnd = rand() % used_set.size();
						iter = used_set.begin();
						advance(iter, rnd);
						std::cout << "sending " << *iter <<std::endl;
						p_rec->sector = *iter;
						if(p_rec == n->pointers[i]) {
							std::cout << "p_rec == n->pointers[i]" << std::endl;
							n->sectors[i] = *iter;
						}
						else {
							p_rec_old->next_sect = *iter;
							std::cout << "p_rec != n->pointers[i]" << std::endl;
						}
						used_set.erase(*iter);
						if(p_rec_old != NULL){
							std::cout << "serializing" << std::endl;
							last = p_rec_old->sector;
							ser = serialize(p_rec_old,1, &len_end);
							n->pointers[i] = NULL;
							sendData(socketfd, ser, last, len_end);
							delete[] ser;
						}
						p_rec_old = p_rec;
						p_rec = p_rec_old->next;
						std::cout << "done sending " << *iter <<std::endl;
					}
					if(p_rec_old != NULL){
						std::cout << "serializing" << std::endl;
						last = p_rec_old->sector;
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
	//std::cout << "serializing info" << std::endl;
	serialize_info();
	//std::cout << "serialized info" << std::endl;
	//std::cout << " size " << nodes_in_memory.size() << std::endl;
	print_nodes_and_sectors();
	//nodes_in_memory.clear();
	//delete retr;
}
/*
int send_tree(node * root) {
	sect_type sector;
	std::set<sect_type>::iterator iter;
	std::list<node *> remaining;
	int rnd = rand() % used_set.size();
	iter = used_set.begin();
	advance(iter, rnd);
	root->sector = *iter;
	used_set.erase (iter);
}*/

void * retrieveRoot(void) {
	node * root;
	unsigned char * n;
	uint32_t len;
	deserialize_info();
	//std::cout << "retrieved all" << std::endl;

	//retr = (info *)retrieveData(socketfd, 0);
	std::cout << retr->root_sect << " " << retr->last_sect_used <<" " << retr->num_fields<< std::endl;
	//std::cout << "retrievedInfo\n";
	len = retrieveData(socketfd, retr->root_sect, (void **)&n);
	std::cout << "length " << len << std::endl;
	BIO_dump_fp (stdout, (const char *)n, len);
	root = (node *)deserialize(n, 0, len);
	//std::cout << root->sector << std::endl;
	//std::cout << root->num_keys << std::endl;
	used_set.insert(root->sector);
	//std::cout << " root sector " << root->sector << std::endl;
	nodes_in_memory[root->sector] = root;
	//std::cout << "retrievedRoot\n";
	return root;
}	

/*
	
int main(int argc, char *argv[]) {
	node * root;
	srand (time(NULL));
	serverConnectionInit(argv[1], argv[2], &socketfd);
	if((root = (node *)retrieveRoot()) == NULL)
		exit (1);
	//here I have to implement the queries
}
*/
