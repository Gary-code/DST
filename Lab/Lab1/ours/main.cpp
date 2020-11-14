#include"scp_t.h"

using namespace std;

int main() 
{
	scp_t* A = scp_create(2 * 1024);
	scp_info_t* Data1 = (scp_info_t*)scp_alloc(A, 2 * 1024);
	scp_info_t* Data2 = (scp_info_t*)scp_alloc(A, 1024 * 1024);
	Data1->age = 36;
	strcpy_s(Data1->name, "Alexander");
	strcpy_s(Data1->data, "?");
	strcpy_s(Data1->origin, "Guangdong");

	Data2->age = 36;
	strcpy_s(Data2->name, "Gary");
	strcpy_s(Data2->data, "?");
	strcpy_s(Data2->origin, "Guangdong");
	
	printf("%s\n%s\n%s\n%d\n\n", Data1->name, Data1->data, Data1->origin, Data1->age);
	printf("%s\n%s\n%s\n%d\n\n", Data2->name, Data2->data, Data2->origin, Data2->age);

/*	
	cout << Data1->name << endl
		<< Data1->age << endl
		<< Data1->data << endl
		<< Data1->origin << endl<<endl;

	cout << Data2->name << endl
		<< Data1->age << endl
		<< Data2->data << endl
		<< Data2->origin << endl;
*/
	free(Data1);
	free(Data2);
	scp_destroy(A);
	return 0;
}