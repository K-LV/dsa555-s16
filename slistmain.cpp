#include "slist.h"
#include <iostream>
using namespace std;

int main(void){

	SList<int> list;
	SList<int>::iterator i;
	list.insertFront(5);
	list.insertFront(3);
	for(i=list.begin();i!=list.end();i++){
		cout << * i << endl;
	}
	return 0;
}