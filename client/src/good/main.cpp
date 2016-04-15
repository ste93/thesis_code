#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <gmp.h>
#include <string.h>
#include "header_main.h"
#include "lic.h"
#include "utils_tree.h"
#include "creation.h"
#include "insertion.h"
#include "cli_server.h"
#include "aes.h"
#include "cli_socket.h"
//#include "../../bigint/Library.hh"

using namespace std;
int main( int argc, char ** argv ) {
	//main variable declaration
	fields_type  * fields_array;
	clock_t startTime;
	FILE *fp;
	node *root;
	fstream queryFile;
	node * no;
	record * re;
	int num_fields, key_pos, i;
	char instruction, where_data, new_or_not;
	char license_part;
	std::string line;
	initaes();
	int numberOfQueries = 0;
	mpz_t totalTime;
	mpz_init(totalTime);
	mpz_t dataSent;
  	mpz_init(dataSent);
	srand(time(NULL));
	root = NULL;
	verbose_output = false;
	//connection init
	if (argc == 3){
		if (serverConnectionInit(argv[1], argv[2], &socketfd) != 0) {
			printf("failed to contact the server");
			exit(1);
		}
	}
	//the number of arguments is not valid
	else {
		printf("usage_1()");
		exit(1);
	}
	//begin to interact with user
	std::cout << "do you wanna create a new database or editing/searching on an existing one? (n = new, e = existing): ";
	std::cin >> new_or_not;
		//the user wants to create a new tree
		if (new_or_not == 'n') {
			retr = new info;
			std::cout << "insert the number of fields you want to set: ";
			std::cin >> num_fields;
			retr->num_fields = num_fields;
			std::cout << "do you wanna take the database from a file or you prefer to insert it manually? (f = file; m = manually): ";
			std::cin >> where_data;
			if (where_data == 'f'){
				mpz_set_ui(totalTime, 0);
				time_t startTime = clock();
				std::cout << "asdfad " << startTime <<  std::endl;

				root = read_from_file(root, num_fields);
				std::cout << "asdfad" << std::endl;
				mpz_t time;
				mpz_init(time);
				mpz_t clk;
				mpz_init(clk);
				std::cout << "asdfad" << std::endl;
				mpz_set_ui(clk, clock());
				mpz_sub_ui(time, clk, startTime);
				std::cout << time << std::endl;
				mpz_set_ui(clk,(int)CLOCKS_PER_SEC);
				mpz_div(time, time,clk);
				std::cout << time << std::endl;
			}
			else if(where_data == 'm') {			
				std::cout << "type i for inserting the data\n";
			}
		}
	while (scanf("%c", &instruction) != EOF) {
		switch (instruction) {
		case 'q':
			{
				time_t startTime;
				mpz_set_ui(totalTime, 0);
				startTime= clock();
				send_tree(root);
				mpz_t time;
				mpz_init(time);
				mpz_t clk;
				mpz_init(clk);
				std::cout << "asdfad" << std::endl;
				mpz_set_ui(clk, clock());
				mpz_sub_ui(time, clk, startTime);
				gmp_printf("the time is %Z", time);
				std::cout << std::endl;
				mpz_set_ui(clk, CLOCKS_PER_SEC);
				mpz_div(time, time,clk);
				std::cout << time << std::endl;
				root = NULL;
			}
			break;
		case 't':
			if (root != NULL) {
				std::cout << root->num_keys << std::endl;
				for(i=0;i<root->num_keys;i++) {
					std::cout << root->keys[i] << " ";
					std::cout << root->sectors[i] << " " << root->pointers[i] << " ; ";	
				}
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
			//root_ptr = root;
			fields_array = new fields_type[num_fields];
			for (i=0;i<num_fields;i++) {
				std::cin >> fields_array[i];
				std::cout << fields_array[i];
			}
			temp_key = fields_array[0];
			root = insert(root, temp_key, fields_array);
			//root_ptr = root;
			for(i=0;i<=root->num_keys;i++)
				std::cout << root->keys[i];
			break;
		case 'p':
			key_type key_tmp;
			if (root == NULL) 
				root = (node *)retrieveRoot();
			//root_ptr = root;
			std::cin >> key_tmp;
			find_and_print(root, key_tmp, false);
			std::cout << "end " << std::endl;
			break;
		case 'r':
			int32_t key1, key2;
			if (root == NULL) 
				root = (node *)retrieveRoot();
			//root_ptr = root;
			std::cout << "inserisci la chiave!!" << std::endl;
			std::cin >> key1;
			//std::cout << "ciao"<< std::endl;
			std::cin >> key2;
			//std::cout << "range "<< std::endl;
			find_and_print_range(root, key1, key2, false);
			break;
		case 'c':
			//write all data temporary saved in local memory to the server and close the connection 
			serverConnectionFinish(socketfd);
			return 0;
		case 'a':
			if (root == NULL) 
				root = (node *)retrieveRoot();
			//root_ptr = root;
			root = read_from_file(root, num_fields);
			break;
		case 'f':
			std::string queryFileName;
			std::string query;
			cout << "Enter a filename for a file that contains queries (type exit to end): ";
			cin >> queryFileName;

			queryFile.open(queryFileName.c_str());
			getline(queryFile, query);

			cout << query << endl; //heading
/*
			totalTime = 0;
			dataSent = 0;
			numberOfQueries = 0;
			dimension = 1;
*/
			if (root == NULL) 
				root = (node *)retrieveRoot();

			cout << "Starting to process queries" << endl;
			while (getline(queryFile, query))
			{
				if (query.length() > 1) //last line check
				{
					numberOfQueries++;
					//startTime = clock();
					//dataSent = client.processQuery(query, dimension);
					std::stringstream stream(query);
					//while(1) {
					int n, k;
					stream >> n;
					stream >> k;
					//if (!stream)
					//	break;
					std::cout << "Found integer: " << n << k << std::endl;
					find_and_print_range(root, n, k, false);


					//}
					std::cout << std::endl;
/*					mpz_t time;
					time = (mpz_t)( clock() - startTime ) / (mpz_t)CLOCKS_PER_SEC;
					//cout << "Query #" << numberOfQueries << " is done in " << time << " s." << endl;
					//timeOfQueries.push_back(time);
					totalTime += time;*/
				}
				
			}/*
			cout << "time to process all queries: " << totalTime << " seconds" << endl;
            //cout << "average time to process a query: " << totalTime/(mpz_t)numberOfQueries << " seconds." << endl;
            cout << "average data sent (and received): " << dataSent / (mpz_t)numberOfQueries << " bytes." << endl;*/
			break;
		default:
			usage_2();
			break;
		}
		while (getchar() != (int)'\n');
		printf("> ");
	}
}
