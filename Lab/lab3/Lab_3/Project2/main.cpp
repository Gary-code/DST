#include "scp_htindex.h"
#include<iostream>
using namespace std;

int main()
{
	scp_create_bpus(4);
	cout << scp_get_by_ordersn(111)->ordersn;

	system("pause");
	return 0;
}