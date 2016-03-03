#include "/home/ste/critto/client/b+tree/stx-btree/include/stx/btree.h"
#include "/home/ste/critto/client/b+tree/stx-btree/testsuite/tpunit.h"
#include <sys/types.h>
#include <db.h>
#include <stx/btree_multiset.h>
#include <stx/btree_multimap.h>
#include <stx/btree_map.h>
#include <iostream>
#include <cstdlib>
#include <inttypes.h>

//#include <btree.h>
//#include "/usr/src/loinux-headers-3.16.0-4-common/include/linux/btree.h"


/* header file */
typedef struct data {
	int index;
	int sector;
	int key;
	int value1;
	int value2;
	int value3;
} data;

typedef struct pair {
	int p1;
	data p2;
} pair;


struct testcomp
{
	unsigned int somevalue;

	explicit testcomp(unsigned int sv)
		: somevalue(sv)
	{ }

	bool operator () (const struct testdata& a, const struct testdata& b) const
	{
		return a.a > b.a;
	}
};

struct traits_nodebug : stx::btree_default_set_traits<KeyType>
{
static const bool selfverify = true;
	static const bool debug = false;

	static const int  leafslots = 8;
	static const int  innerslots = 8;
};


//template class stx::btree_map<int, double>;



typedef stx::btree_map<struct testdata, struct testcomp, struct traits_nodebug<struct testdata> > btree_type;



int main() {
	stx::btree_map<int, double> bt1;
	std::cout << bt1.m_root;
	//bt1->insert(4.0,4.0);
	return 0;
}
