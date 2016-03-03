#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;


typedef struct element {
	int pos;
	char *value;
	struct element *next;
} element;

int delete_list(element * list_first) {
	element *tmp = list_first, *succ;
	while(tmp!=NULL) {
		succ = tmp->next;
		delete(tmp);
		tmp = succ;
	}
	return 0;
}

element * insert_element(element * root, int index, string value) {
	element * tmp, *tmp2;
	tmp = new element;
	tmp->pos = index;
	int c = value.size();
	cout << c;
	tmp->value = (char *)malloc(c+1);
	value.copy(tmp->value, c, 0);
	if (root == NULL || tmp->pos < root->pos) {
		tmp->next = root;
		root = tmp;
	}
	else {
		tmp2 =  root;
		if (tmp2->next != NULL) {
			while (tmp2->next != NULL && tmp2->next->pos < tmp->pos) {
				tmp2 = tmp2->next;
			}
		}
		
		if (tmp2->next != NULL && tmp2->next->pos == tmp->pos){
			free(tmp2->next->value);
			tmp2->next->value = (char *)malloc(c+1);
			value.copy(tmp2->next->value, c, 0);
			delete tmp;
		}
		else if(tmp2->next == NULL && tmp2->pos == tmp->pos) {
			free(tmp2->value);
			tmp2->value = (char *)malloc(c+1);
			value.copy(tmp2->value, c, 0);
			delete tmp;
		}
		else {
			tmp->next = tmp2->next;
			tmp2->next = tmp;
		}

	}
	return root;
}

void print_list(element * root){
	element * list_ptr = root;
	while(list_ptr != NULL){
		cout << list_ptr->pos << " " << list_ptr->value << " ; ";
		list_ptr = list_ptr->next;
	}
	cout << "\n";
}

/*
int main() {
	element * root = NULL;
	int x;
	string y;
	for(int i =0 ; i<5 ; i++){
		cin >> x;
		cin >> y;
		root = insert_element(root, x, y);
		print_list(root);
	}
	if(delete_list(root) ==0){
		cout << "aaa";
	}
}
		
*/
