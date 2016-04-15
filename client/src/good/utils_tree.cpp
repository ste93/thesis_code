#include "utils_tree.h"
#include "const_cli.h"
#include "insertion.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "const_cli.h"
#include "cli_socket.h"
#include "aes.h"
#include <arpa/inet.h>
#include <string>
#include <fstream>
extern unsigned char key[32];
extern unsigned char iv[16];


extern int order;


/* Helper function for printing the
 * tree out.  See print_tree.
 */
void enqueue( node * new_node ) {
	node * c;
	if (queue == NULL) {
		queue = new_node;
		queue->next = NULL;
	}
	else {
		c = queue;
		while(c->next != NULL) {
			c = c->next;
		}
		c->next = new_node;
		new_node->next = NULL;
	}
}


/* Helper function for printing the
 * tree out.  See print_tree.
 */
node * dequeue( void ) {
	node * n = queue;
	queue = queue->next;
	n->next = NULL;
	return n;
}


void serialize_info(){
	unsigned char *ser, *str;
	uint32_t len;
	int tot_len, length;
	length = sizeof(info);
	tot_len = length + 10 + ((length+10) % 16);
	str = new unsigned char[tot_len];
	retr->root_sect = htonl(retr->root_sect);
	retr->last_sect_used = htonl(retr->last_sect_used);
	retr->num_fields = htonl(retr->num_fields);	
	memcpy(str, retr, length);
	if (!RAND_bytes((unsigned char *)&str[sizeof(info)], 10 + ((sizeof(info)+10) % 16))) {
		return NULL;
	}
	int ciphertext_len;
	unsigned char *n = new unsigned char[512];
	//n = (unsigned char *)memset(n, 0, 512);
	unsigned char iv[16];
	if (!RAND_bytes((unsigned char *)iv, 16)) {
		std::cout << "!rand";
		return NULL; 
	}
	//std::cout << "iv" << std::endl;
	//BIO_dump_fp (stdout, (const char *)iv, 16);
	ciphertext_len = encrypt ((unsigned char *)str,tot_len, key, iv,&n[4]);
	//std::cout << "encrypting" << std::endl;
	memcpy(&n[ciphertext_len+4], iv, 16);
	//BIO_dump_fp (stdout, (const char *)n, 512);
	//BIO_dump_fp (stdout, (const char *)iv, 16);
	len = ciphertext_len + 20;
	//std::cout << len << std::endl;
	//std::cout << len << std::endl;
	len = htonl(len);
	memcpy(n, &len, 4);
	//std::cout << "done serializing" << std::endl;
	//std::cout << "enc" << std::endl;
	//BIO_dump_fp (stdout, (const char *)n, 512);
	sendData(socketfd, n, 0, ciphertext_len + 20);
	//BIO_dump_fp (stdout, (const char *)n, ciphertext_len + 20);
	delete[] n;
}

void deserialize_info() {
	unsigned char * node_char;
	uint32_t len, len_net;
	len = retrieveData(socketfd, 0, (void **)&node_char);
	//std::cout << node_char << std::endl;
	//len = ntohl(len_net);
	//std::cout << "enc" << std::endl;
	//std::cout << "length " << len << " " << len_net << std::endl;
	int i;
	unsigned char iv[16];
	//printf("len = %d\n", len);
	//std::cout << len << std::endl;
	//std::cout << "info retrieved" << std::endl;
	memcpy( iv, &node_char[len-16], 16);
	//std::cout << "iv" << std::endl;
	//BIO_dump_fp (stdout, (const char *)iv, 16);
	unsigned char decryptedtext[512];
	int decryptedtext_len;
	//BIO_dump_fp (stdout, (const char *)node_char, len-16);
	decryptedtext_len = decrypt((unsigned char *)node_char, len-16, key, iv, decryptedtext);
	retr = new info;
	memcpy(retr, decryptedtext, sizeof(info));
	retr->root_sect = ntohl(retr->root_sect);
	retr->last_sect_used = ntohl(retr->last_sect_used);
	retr->num_fields = ntohl(retr->num_fields);	
	delete[] node_char;
}
/* Utility function to give the height
 * of the tree, which length in number of edges
 * of the path from the root to any leaf.
 */
int height( node * root ) {
	int h = 0;
	int i;
	node * c = root;
	while (!c->is_leaf) {
		i = 0;
		while(c->pointers[i] == NULL) {
			if (i < order - 1)
				i++;
			else {
				//this case should exists only when there is no tree in the cache
				retrieveDataFake(socketfd, c->sectors[0], false, c, 0);
				i = 0;
				break;
			}
		}
		c = (node *)c->pointers[i];
		h++;
	}
	return h;
}

/* this function is used just before send all the data to the server
 * so it clears all the pointers because in the server all the pointers must be set to NULL
 * and turns all the integers into network mode so there is no problem for the endianess
 * all the fields are created at the beginning so there's no problem for the serialization
 */
unsigned char * serialize(void * to_serialize, bool is_record, int *len_end) {
	int i;
	int length, tot_len;
	uint32_t len;
	int end;
	unsigned char * str;
	if(is_record == 1) {
		record * record_to_serialize;
		record_to_serialize = (record *) to_serialize;
		record_to_serialize->key = htonl(record_to_serialize->key);
		record_to_serialize->sector = htonl(record_to_serialize->sector);
		record_to_serialize->parent = NULL;
		length = sizeof(record) + (retr->num_fields * sizeof(fields_type)) + 10;
		//std::cout << length << std::endl;
		tot_len = length + 10 + ((length) % 16);
		str = new unsigned char[tot_len];
		//for (i=1;)
		memcpy(str, record_to_serialize, sizeof(record));
		memcpy(&str[sizeof(record)], record_to_serialize->fields, retr->num_fields * sizeof(fields_type));
		delete record_to_serialize->fields;
		delete record_to_serialize;
		if (!RAND_bytes((unsigned char *)&str[length - 10], 10 + (length % 16))) {
			return NULL;
		}
	}
	else {
		node * node_to_serialize;
		node_to_serialize = (node *) to_serialize;
		length = sizeof(node);
		/*end = node_to_serialize->num_keys;
		if (node_to_serialize->is_leaf) 
			end++;*/
		for (i=0;i<DEFAULT_ORDER;i++){
			node_to_serialize->pointers[i] = NULL;
			node_to_serialize->sectors[i] = htonl(node_to_serialize->sectors[i]);
			node_to_serialize->keys[i] = htonl(node_to_serialize->keys[i]);//here I suppose that the key is an integer
		}
		node_to_serialize->parent = NULL;
		node_to_serialize->num_keys = htonl(node_to_serialize->num_keys);
		node_to_serialize->next = NULL; // Used for queue.
		node_to_serialize->sector = htonl(node_to_serialize->sector);//here I suppose that the sector type is uint32_t
		tot_len = length + 10 + ((length+10) % 16);
		str = new unsigned char[tot_len];
		memcpy(str, node_to_serialize, length);
		delete node_to_serialize;
		if (!RAND_bytes((unsigned char *)&str[sizeof(node)], 10 + ((sizeof(node)+10) % 16))) {
			return NULL;
		}
	}

	int ciphertext_len;
	unsigned char *n = new unsigned char[512];
	n = (unsigned char *)memset(n, 0, 512);
	unsigned char iv[16];
	if (!RAND_bytes((unsigned char *)iv, 16)) {
		std::cout << "!rand";
		return NULL; 
	}

	ciphertext_len = encrypt ((unsigned char *)str,tot_len, key, iv,&n[4]);
	memcpy(&n[ciphertext_len+4], iv, 16);
	std::cout << " serialize " << ciphertext_len + 20 << std::endl;
	BIO_dump_fp (stdout, (const char *)n, 512);
	*len_end = ciphertext_len + 20;
	len = ciphertext_len + 20;
	len = htonl(len);
	memcpy(n, &len, 4);
	delete[] str;
	return n;
}
	
/*this function is used just after the client receives the data from the server.
 * it restores all the data and put the pointers when they are there.
 */	
 void * deserialize(unsigned char * node_char, bool is_record, uint32_t len) {
	int i;
	unsigned char iv[16];
	//printf("len = %d\n", len);
	//std::cout << len << std::endl;
	memcpy( iv, &node_char[len-16], 16);
	unsigned char decryptedtext[512];
	int decryptedtext_len;
	decryptedtext_len = decrypt((unsigned char *)node_char, len-16, key, iv, decryptedtext);
	//std::cout << is_record;
	if(is_record) {
		record * rec;
		rec = new record;
		memcpy((void *) rec, decryptedtext, sizeof(record));
		//std::cout << "is_record" <<std::endl;
		rec->key = ntohl(rec->key);
		//std::cout << "key " << rec->key << std::endl;
		rec->sector = ntohl(rec->sector);
		//std::cout << "sector " <<  rec->sector << std::endl;
		rec->parent = NULL;
		fields_type * n = new fields_type[retr->num_fields];
		memcpy(n, &decryptedtext[sizeof(record)], sizeof(fields_type[retr->num_fields]));
		//std::cout << "fields created" << std::endl; 
		rec->fields = n;
		/*for(i=0;i<retr->num_fields;i++)
			std::cout << n[i]<<std::endl;*/
		//std::cout << "done deserializing";
		delete[] node_char;
		return rec;
	}
	else {
		node * node_serialized;
		node_serialized = new node;
		memcpy((void *) node_serialized, decryptedtext, sizeof(node));
		for (i=0;i<DEFAULT_ORDER;i++){
			node_serialized->keys[i] = ntohl(node_serialized->keys[i]);//here I suppose that the key is an integer
			node_serialized->sectors[i] = ntohl(node_serialized->sectors[i]);
			node_serialized->pointers[i] = NULL;//this will be managed later when decided fake searches, but I think is not needed for now.
		}
/*		if(!node_serialized->is_leaf) {
			node_serialized->sectors[i] = ntohl(node_serialized->sectors[i]);
			node_serialized->pointers[i] = NULL;//this will be managed later when decided fake searches, but I think is not needed for now.
		}*/
		node_serialized->parent = NULL; //this will be managed later
		node_serialized->num_keys = ntohl(node_serialized->num_keys);
		node_serialized->next = NULL; // Used for queue so must be left NULL
		node_serialized->sector = ntohl(node_serialized->sector);//here I suppose that the sector type is uint32_t
		delete[] node_char;
		return node_serialized;
	}
}

/* Utility function to give the length in edges
 * of the path from any node to the root.
 */
 //here I always have all the nodes in the cache
int path_to_root( node * root, node * child ) {
	int length = 0;
	node * c = child;
	while (c != root) {
		c = c->parent;
		length++;
	}
	return length;
}


/* Finds the appropriate place to
 * split a node that is too big into two.
 */
int cut( int length ) {
	if (length % 2 == 0)
		return length/2;
	else
		return length/2 + 1;
}

node * read_from_file(node * root, int num_fields) {
	fields_type  * fields_array;
	FILE *fp;
	key_type key_tmp;
	std::string file_to_read; 
	int key_pos, i;
	std::cin >> file_to_read;
	std::cin >> key_pos;
	fp = fopen(file_to_read.c_str(), "r");
	if (fp == NULL) {
		perror("Failure to open input file.");
		exit(EXIT_FAILURE);
	}
	while (!feof(fp)) {
		fields_array = new fields_type[num_fields];
		for(i=0;i<num_fields;i++){
			fscanf(fp, "%d", &fields_array[i]);
			//std::cout << fields_array[i];
			if (i<num_fields - 1)
				fscanf(fp," ");
			else if(i == num_fields-1)
				fscanf(fp,"\n");
		}
		//std::cout << std::endl;
		key_tmp = fields_array[key_pos - 1];
		fields_array[key_pos - 1] = fields_array[0];
		fields_array[0] = key_tmp;
		root = insert(root, key_tmp, fields_array);
		//std::cout << "after inserting" <<std::endl;
	}
	fclose(fp);
	return root;
}
