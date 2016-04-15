/* 
 *  bpt.c  
 */
/*
 *
 *  bpt:  B+ Tree Implementation
 *  Copyright (C) 2010  Amittai Aviram  http://www.amittai.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  Author:  Amittai Aviram 
 *    http://www.amittai.com
 *    amittai.aviram@gmail.edu or afa13@columbia.edu
 *    Senior Software Engineer
 *    MathWorks, Inc.
 *    3 Apple Hill Drive
 *    Natick, MA 01760 
 *  Original Date:  26 June 2010
 *  Last modified: 15 April 2014
 *
 *  This implementation demonstrates the B+ tree data structure
 *  for educational purposes, includin insertion, deletion, search, and display
 *  of the search path, the leaves, or the whole tree.
 *  
 *  Must be compiled with a C99-compliant C compiler such as the latest GCC.
 *
 *  Usage:  bpt [order]
 *  where order is an optional argument
 *  (integer MIN_ORDER <= order <= MAX_ORDER)
 *  defined as the maximal number of pointers in any node.
 *
 */

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS

#include <map>
#include "header_cli.h"
#include "lic.h"
#include "utils_tree.h"
#include "creation.h"
#include "deletion.h"
#include "insertion.h"
#include "client_main.h"

// FUNCTION PROTOTYPES.

// MAIN

int main( int argc, char ** argv ) {

	char * input_file;
	FILE * fp;
	node * root;
	int input, range2;
	char instruction;
	char license_part;

	root = NULL;
	verbose_output = false;
/*
	if (argc > 1) {
		order = atoi(argv[1]);
		if (order < MIN_ORDER || order > MAX_ORDER) {
			fprintf(stderr, "Invalid order: %d .\n\n", order);
			usage_3();
			exit(EXIT_FAILURE);
		}
	}

	license_notice();
	usage_1();
	usage_2();
*/
	if (argc > 2) {
		switch (argc[1]) {
			case 's';
				
		}
				
		/*input_file = argv[2];
		fp = fopen(input_file, "r");
		if (fp == NULL) {
			perror("Failure to open input file.");
			exit(EXIT_FAILURE);
		}
		while (!feof(fp)) {
			fscanf(fp, "%d\n", &input);
			root = insert(root, input, input);
		}
		fclose(fp);
		print_tree(root);*/
	}

	printf("> ");
	while (scanf("%c", &instruction) != EOF) {
		switch (instruction) {
		case 'd':
			scanf("%d", &input);
			root = delete_key(root, input);
			print_tree(root);
			break;
		case 'i':
			scanf("%d", &input);
			root = insert(root, input, input);
			print_tree(root);
			break;
		case 'f':
		case 'p':
			scanf("%d", &input);
			find_and_print(root, input, instruction == 'p');
			break;
		case 'r':
			scanf("%d %d", &input, &range2);
			if (input > range2) {
				int tmp = range2;
				range2 = input;
				input = tmp;
			}
			find_and_print_range(root, input, range2, instruction == 'p');
			break;
		case 'l':
			print_leaves(root);
			break;
		case 'q':
			while (getchar() != (int)'\n');
			return EXIT_SUCCESS;
		case 's':
			if (scanf("how %c", &license_part) == 0) {
				usage_2();
				break;
			}
			switch(license_part) {
			case 'w':
				print_license(LICENSE_WARRANTEE);
				break;
			case 'c':
				print_license(LICENSE_CONDITIONS);
				break;
			default:
				usage_2();
				break;
			}
			break;
		case 't':
			print_tree(root);
			break;
		case 'v':
			verbose_output = !verbose_output;
			break;
		case 'x':
			if (root)
				root = destroy_tree(root);
			print_tree(root);
			break;
		default:
			usage_2();
			break;
		}
		while (getchar() != (int)'\n');
		printf("> ");
	}
	printf("\n");

	return EXIT_SUCCESS;
}
