#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
using namespace std;



int readFile(string fileName) {
	string line;
	string temp;
	ifstream filedb;
	filedb.open(fileName);
	int i;
	if (!filedb.is_open())
		return -1;
    while ( getline(filedb,line) )
    {
    	cout << line << '\n';
		istringstream iss(line);
		vector<string> tokens{istream_iterator<string>{iss},istream_iterator<string>{}};
		cout << tokens[0]
		map_values = new std::map<int, string>
		i = 0;
		for(string n : tokens){
			cout << "inserting" << n << endl;
			n.copy(map_values[i], 0, sizeof(n))
			i++;
		}		
	}
	filedb.close();
	return 0;
}
