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

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}

using namespace std;
int main( int argc, char ** argv ) {
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
			if(root!=NULL)
			{
				time_t startTime;
				mpz_set_ui(totalTime, 0);
				startTime= clock();
				send_tree(root);
				mpz_t time;
				mpz_init(time);
				mpz_t clk;
				mpz_init(clk);
				localtime(&startTime);
				mpz_set_ui(time, startTime);				
				std::cout << "asdfad" << std::endl;
				gmp_printf("the time is %Zd", time);
				mpz_set_ui(clk, clock());
				std::cout << clock() << std::endl;
				gmp_printf("this is %Zd", clk);
				mpz_sub_ui(time, clk, startTime);
				gmp_printf("the time is %Zd", time);
				std::cout << std::endl;
				//mpz_set_ui(clk, CLOCKS_PER_SEC);
				std::cout << CLOCKS_PER_SEC << std::endl;
				mpz_cdiv_q_ui(time, time, CLOCKS_PER_SEC);
				gmp_printf("the time is %Zd", time);
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
			std::cout << "end " << std::endl;
			break;
		case 'r':
			int32_t key1, key2;
			if (root == NULL) 
				root = (node *)retrieveRoot();
			std::cout << "inserisci la chiave!!" << std::endl;
			std::cin >> key1;
			std::cin >> key2;
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
/*		case 'f':
			std::string fileQuery;
			std::string queryLine;
			cout << "Enter a filename for a file that contains queries (type exit to end): ";
			cin >> fileQuery;

			queryFile.open(fileQuery.c_str());
			getline(queryFile, queryLine);

			cout << queryLine << endl; //heading
			if (root == NULL) 
				root = (node *)retrieveRoot();

			cout << "Starting to process queries" << endl;
			time_t startTime;
			mpz_set_ui(totalTime, 0);
			startTime= clock();
			send_tree(root);
			mpz_t time;
			mpz_init(time);
			mpz_t clk;
			mpz_init(clk);
			mpz_set_ui(clk, clock());
			gmp_printf("this is %Zd", clk);
			while (getline(queryFile, queryLine))
			{
				if (queryLine.length() > 1) //last line check
				{
					numberOfQueries++;
					std::stringstream stream(queryLine);
					int n, k;
					stream >> n;
					stream >> k;
					std::cout << "Found integer: " << n << k << std::endl;
					find_and_print_range(root, n, k, false);


					//}
					std::cout << std::endl;
					mpz_t time;
					time = (mpz_t)( clock() - startTime ) / (mpz_t)CLOCKS_PER_SEC;
					//cout << "Query #" << numberOfQueries << " is done in " << time << " s." << endl;
					//timeOfQueries.push_back(time);
					totalTime += time;
				}
				
			}
			break;*/
		default:
			usage_2();
			break;
		}
		while (getchar() != (int)'\n');
		printf("> ");
	}
}
