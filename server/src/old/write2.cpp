#include "constant.h"
#include <iostream>
#include <stdio.h>

using namespace std;

int write_sector(void * cont, int sect_num)
{

    //char block[SECTOR_SIZE] = "this is a try for write sector";
	//int length=0;
    //cout<<"Implementation of the File Handler Read Method..."<<endl;

	int s;
    FILE *f = fopen(DISK, "w");
    if(f == NULL)
    {
        cout<<"Error In Opening the HardDisk File Retuning Error..."<<endl;
        return -1;
    }

    //Read One Block of Data to Buffer
    if(fwrite(cont, SECTOR_SIZE, sect_num, f) <= 0) {
		cout<< s << "Error writing"<<endl;
        return -1;
	}
	cout << "write";
    /* Do something with the data 
    cout<<"Length : "<<length<<endl;
	cout << block;*/

    return 0;
}

