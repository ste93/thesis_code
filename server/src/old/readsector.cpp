#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

    char block[512];
    int length=0;
    cout<<"Implementation of the File Handler Read Method..."<<endl;

    FILE *f = fopen(argv[1], "r");
    if(f == NULL)
    {
        cout<<"Error In Opening the HardDisk File Retuning Error..."<<endl;
        return -1;
    }

    //Read One Block of Data to Buffer
    length = fread(block, 1, sizeof(block), f);

    /* Do something with the data */
    cout<<"Length : "<<length<<endl;
	cout << block;
    return 0;
}
