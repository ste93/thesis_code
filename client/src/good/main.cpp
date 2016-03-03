#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include "header_main.h"
#include "lic.h"
#include "utils_tree.h"
#include "creation.h"
#include "insertion.h"
#include "cli_server.h"
#include "aes.h"
#include "cli_socket.h"


int main( int argc, char ** argv ) {
	fields_type  * fields_array;
	FILE *fp;
	node *root;
	node * no;
	record * re;
	int num_fields, key_pos, i;
	char instruction, where_data, new_or_not;
	char license_part;
	std::string line;
	initaes();
    srand(time(NULL));
	
	root = NULL;
	verbose_output = false;

	if (argc >= 3){
		if (serverConnectionInit(argv[1], argv[2], &socketfd) != 0) {
			printf("failed to contact the server");
			exit(1);
		}
		//std::cout << argc << argv[0] << argv[1] << argv[2]
	}
	else {
		printf("usage_1()");
		exit(1);
	}
	std::cout << "do you wanna create a new database or editing/searching on an existing one? (n = new, e = existing): ";
	std::cin >> new_or_not;

		if (new_or_not == 'n') {
			//delete the previous tree and create a new tree with the data inserted by the user
			retr = new info;
			std::cout << "insert the number of fields you want to set: ";
			std::cin >> num_fields;
			retr->num_fields = num_fields;
			std::cout << "do you wanna take the database from a file or you prefer to insert it manually? (f = file; m = manually): ";
			std::cin >> where_data;
			if (where_data == 'f'){
				root = read_from_file(root, num_fields);
				//print_tree(root);
				/*key_tmp = fields_array[key_pos - 1];
				//fields_array = new std::vector<int>;
				std::ifstream myfile(file_to_read.c_str(), std::ifstream::in);
				while (getline(myfile, line)) {
					root = insert(root, key_tmp, fields_array);
				}
				fclose(fp);
				//break;*/
			}
			else if(where_data == 'm') {
				int i;
				/*for (i=0;i<num_fields;i++) {
					std::cin >> fields_array[i];
				}
				std::cout << "end\n";	
				key_tmp = fields_array[0];*/
				//root = insert(root, key_tmp, fields_array);
				//break;
			
				std::cout << "type i for inserting the data\n";
			}
		}
	//}
	while (scanf("%c", &instruction) != EOF) {
		switch (instruction) {
		case 'e':
			//unsigned char * n;
			/*int sect;
			retr->root_sect = root->sector;
			sect = root->sector;
			//std::cout << "zsdfadsfadfa" << retr->root_sect << " " << retr->last_sect_used << " " << retr->num_fields << std::endl;
			serialize_info();
			n = serialize(root, 0);
			//BIO_dump_fp (stdout, (const char *)n, 512);
			sendData(socketfd, n, sect, 512);			
			//std::cout << "done_serialization" << std::endl;
			//free(retr);
			retr = NULL;
			root = NULL;
			//free(root);
			//std::cout << "ciao " << std::endl;
			root = (node *)retrieveRoot();
			//std::cout << "afsdfashhtyhbtd" << retr->root_sect << " " << retr->last_sect_used << " " << retr->num_fields << std::endl;
			
			int sect;
			bool is_record;
			std::cin >> sect; 
			std::cin >> is_record;
			unsigned char * n, *m;
			node * no, *aa;
			record * rec;
			rec = (record *)nodes_in_memory[sect];
			int length;
			std::cout << "aaa" << rec;
			printf("ssdsa");
			n = serialize(rec, is_record);
			sendData(socketfd, n, sect, 512);
			  BIO_dump_fp (stdout, (const char *)n, 512);
			//memcpy(n,(void *) root, sizeof(node));
			printf("ciao");
			rec = NULL;
			std::cout << std::endl;
			m = (unsigned char *)retrieveData(socketfd, sect);
			  BIO_dump_fp (stdout, (const char *)m, 512);
			rec = (record  *)deserialize(n, is_record);
			//aa = root;
			std::cout << "after deserialize";
			print_record(rec);
			/*
			std::cout << aa->num_keys << " keys ";
				for(i=0;i<aa->num_keys;i++) {
					std::cout << i << " " << aa->keys[i] << " ";
					std::cout << aa->sectors[i] << " asdfa ";
				}
				if (!no->is_leaf) {
					std::cout << aa->keys[i] << " ";
					std::cout << aa->sectors[i] << " asdfa ";
				}
				else {
					std::cout << " is a leaf \n";
				}
				std::cout << std::endl << aa->sector;
				if (root != NULL) {
					for(i=0;i<root->num_keys;i++) {
						std::cout << root->keys[i] << " ";
						std::cout << root->sectors[i] << " ; ";
					}
					std::cout << root->keys[i] << " ";
					std::cout << root->sectors[i] << " ; ";
					std::cout << std::endl << root->sector;
				}*/
				break;
		case 'q':
			send_tree(root);
			root = NULL;
			break;
		case 't':
			if (root != NULL) {
				std::cout << root->num_keys << std::endl;
				for(i=0;i<root->num_keys;i++) {
					std::cout << root->keys[i] << " ";
					std::cout << root->sectors[i] << " " << root->pointers[i] << " ; ";	
				}
				/*std::cout << root->keys[i] << " ";
				std::cout << root->sectors[i] << " ; ";*/
				std::cout << std::endl << root->is_leaf << std::endl;
				std::cout << std::endl << root->sector;
			}
			else {
				root = (node *)retrieveRoot();
			}
			break;
		case 's':
			int se;
			int is_rec;
			std::cin >> se;
			std::cin >> is_rec;
			std::cout << "ok\n";
			if (is_rec == 0) {
				std::map<sect_type, void*>::const_iterator it = nodes_in_memory.find(se);
				if (it!=nodes_in_memory.end()) 
					no = (node *)nodes_in_memory[se];
				//(node *)retrieveDataFake(socketfd, se, false);
				std::cout << no->num_keys << " keys ";
				for(i=0;i<no->num_keys;i++) {
					std::cout << i << " " << no->keys[i] << " ";
					std::cout << no->sectors[i] << " asdfa ";
				}
				if (!no->is_leaf) {
					std::cout << no->keys[i] << " ";
					std::cout << no->sectors[i] << " asdfa ";
				}
				else {
					std::cout << " is a leaf \n";
				}
				std::cout << std::endl << no->sector; 
			}
			else {
				std::map<sect_type, void*>::const_iterator it = nodes_in_memory.find(se);
				if (it!=nodes_in_memory.end()) 
					re = (record *)nodes_in_memory[se];
				print_record(re);
			} 
			break;
		case 'm':
			print_nodes_and_sectors();
			break;
		case 'i':
			key_type temp_key;
			if (root == NULL) 
				root = (node *)retrieveRoot();
			fields_array = new fields_type[num_fields];
			for (i=0;i<num_fields;i++) {
				std::cin >> fields_array[i];
				std::cout << fields_array[i];
			}
			temp_key = fields_array[0];
			root = insert(root, temp_key, fields_array);
			for(i=0;i<=root->num_keys;i++)
				std::cout << root->keys[i];
			break;
		case 'p':
			key_type key_tmp;
			if (root == NULL) 
				root = (node *)retrieveRoot();
			std::cin >> key_tmp;
			find_and_print(root, key_tmp, false);
			break;
		case 'r':
			key_type key1, key2;
			if (root == NULL) 
				root = (node *)retrieveRoot();
			std::cin >> key1;
			std::cin >> key2;
			std::cout << "range ";
			find_and_print_range(root, key1, key2, false);
			break;
		case 'c':
			//write all data temporary saved in local memory to the server and close the connection 
			serverConnectionFinish(socketfd);
			return 0;
		case 'a':
			if (root == NULL) 
				root = (node *)retrieveRoot();
			root = read_from_file(root, num_fields);
			break;
		default:
			usage_2();
			//std::cout << used_set.max_size() << std::endl;
			break;
		}
		while (getchar() != (int)'\n');
		printf("> ");
	}
}
