/*********************************************************************/
/*                                                                   */
/* Tester for DSA555-s16 assignment 1, question 3                    */
/* Version 1.0                                                       */
/* To compile:                                                       */
/*                                                                   */
/*      g++ a1q3main.cpp -std=c++0x                                  */
/*                                                                   */
/* for memory leak check                                             */
/* valgrind --leak-check=full --track-origins=yes a.out              */
/* for vs: google for vld.h                                          */
/*********************************************************************/

#include "sortedlist.h"
#include <cstring>
#include <iostream>
using namespace std;
//change the #define VERBOSE to 1 for a bit more error information
#define VERBOSE 1

struct Record{
	char word_[30];
	int count_;
};
ostream& operator<<(ostream& os, const Record rec){
	os << rec.word_;
	return os;
}
bool operator==(const Record& a,const Record& b){
	bool rc=false;
	if(strcmp(a.word_,b.word_)==0 && a.count_==b.count_){
		rc=true;
	}
	return rc;
}
bool operator!=(const Record& a,const Record& b){
	return !(a==b);
}
bool operator<(const Record& a,const Record& b){
	return strcmp(a.word_,b.word_) < 0;
}
bool operator>(const Record& a,const Record& b){
	return strcmp(a.word_,b.word_) > 0;
}
template <class T>
void removeItem(T arr[],int idx,int sz);
template <class T>
bool checkList(SortedList<T>& list,const T arr2[],int sz);
template <class T>
void duplicateArray(T dest[], const T src[],int sz);
template <class T>
void printLists(SortedList<T>& list,const T array[],int sz);
template <class T>
void removeRange(T arr[],int from,int to, int sz);
template <class T>
void mergeArray(T arr[],T tmp[],int startA,int startB,int endB);
template <class T>
void mergeSort(T arr[],T tmp[],int start,int end);
template <class T>
void mergeSort(T arr[],int size);

int main(void){
	const int cap=10000;
	const int numSearches=200;
	Record data[20]={
		{"the",1},
		{"quick",2},
		{"brown ",3},
		{"fox",4},
		{"jumped",5},
		{"over",6},
		{"lazy",7},
		{"dog",8},
		{"Calvin",9},
		{"and",10},
		{"Hobbes",11},
		{"night",12},
		{"watch",13},
		{"captain",14},
		{"carrot",15},
		{"lilac",16},
		{"lavender",17},
		{"lily",18},
		{"coffee",19},
		{"tea",20}
	};
	int intData[cap];

	//these array will mirror what happens to LL
	Record mirror[20];
	int intMirror[cap];



	SortedList<Record> recList;
	SortedList<int> intList;

	bool passtest=true;
	int numPassed=0;


	/* Test constructors, begin and end functions*/
	cout << "test 1: create list empty list, check begin() and end()" << endl;
	if((recList.begin() != recList.end()) || 
		(intList.begin() != intList.end())){
		cout << "error 1: check your constructor, begin() and end() functions" << endl;
		passtest=false;
	}
	else{
		numPassed++;
	}
	int idx=0;
	cout << "test 2: create list, insert some records." << endl;

	for(int i=0;i<20;i++){
		recList.insert(data[i]);
		mirror[idx++]=data[i];

	}
	mergeSort(mirror,20);
	for(int i=0;i<cap;i++){
		intMirror[i]=intData[i]=rand();
		intList.insert(intMirror[i]);
	}
	mergeSort(intMirror,cap);
	if(!checkList(recList,mirror,20)){
		passtest=false;
		cout << "Error 2a: Bug in either constructor, insert()or iterator (operator++(int) or operator*)"<< endl;
		#if VERBOSE >= 1
		printLists(recList,mirror,20);
		#endif		
	}
	if(!checkList(intList,intMirror,cap)){
		passtest=false;
		cout << "Error 2b: Bug in either constructor, insert()or iterator (operator++(int) or operator*)"<< endl;
	}
	if(passtest){
		numPassed++;
		cout << "test 3: check iterator, postfix ++ and -- operators"  << endl;
		SortedList<Record>::iterator it=recList.begin();
		for(int i=0;i<19 && passtest;i++){
			Record r=*(it++);
			if(mirror[i] != r){
				passtest=false;
				cout << "Error 3a: postfix ++ operator is not returning iterator to correct node" << endl;
				#if VERBOSE >= 1
				cout << "correct: " << mirror[i] << " ,your list: " << r << endl;
				#endif		
			}
		}
		for(int i=19;i>0 && passtest;i--){
			Record r=*(it--);
			if(mirror[i] != r){
				passtest=false;
				cout << "Error 3b: postfix -- operator is not returning iterator to correct node" << endl;
				#if VERBOSE >= 1
				cout << i << ": correct: " << mirror[i] << " ,your list: " << r << endl;
				#endif		
			}
		}
		for(int i=0;i<19 && passtest;i++){
			if(*(++it) != mirror[i+1]){
				passtest=false;
				cout << "Error 3c: prefix ++ operator is not returning iterator to correct node" << endl;
			}
		}
		for(int i=19;i>0 && passtest;i--){
			if(*(--it) != mirror[i-1]){
				passtest=false;
				cout << "Error 3d: prefix -- operator is not returning iterator to correct node" << endl;
			}
		}
	}

	if(passtest){
		numPassed++;
		cout << "test 4: test find() function"  << endl;
		SortedList<Record>::iterator loc;
		SortedList<int>::iterator loc2;
		for(int i=0;passtest && i<20;i++){
			loc=recList.find(data[i]);
			if(*loc != data[i]){
				passtest=false;
			}
		}
		for(int i=0;passtest && i<20;i++){
			loc2=intList.find(intData[i]);
			if(*loc2 != intData[i]){
				passtest=false;
			}
		}
		if(!passtest){
			cout << "Error 4: find function does not find matching record" << endl;
		}
	}

	SortedList<Record> recCopy=recList;
	SortedList<int> intCopy=intList;
	if(passtest){
		numPassed++;
		cout << "test 5: create a duplicate of the lists with copy constructor, ensure they match" << endl;
		if(!checkList(recList,mirror,20)){
			passtest=false;
			cout << "Error 5a: Bug in copy constructor"  << endl;  		
		}
		if(!checkList(intList,intMirror,cap)){
			passtest=false;
			cout << "Error 5b: Bug in copy constructor" << endl;
		}
		duplicateArray(data,mirror,20);
		duplicateArray(intData,intMirror,cap);
	}
	if(passtest){
		numPassed++;
		cout << "test 6: test erase(iterator) function"  << endl;
		SortedList<Record>::iterator it=recList.begin();
		SortedList<Record>::iterator rc;
		rc=recList.erase(it++);
		removeItem(mirror,0,20);
		if(it!=rc || !checkList(recList,mirror,19)){
			passtest=false;
			cout << "Error 6a: Bug in erase.  iterator was pointing at first item" << endl;
			#if VERBOSE >= 1
			printLists(recList,mirror,19);
			#endif
		}
		it=recList.begin();
		for(int i=0;i<18;i++){
			it++;
		}
		rc=recList.erase(it);
		removeItem(mirror,18,19);
		if(rc!=recList.end() || !checkList(recList,mirror,18)){
			passtest=false;
			cout << "Error 6b: Bug in erase. iterator was pointing at last item" << endl;
			#if VERBOSE >= 1
			printLists(recList,mirror,18);
			#endif
		}
		for(int i=0;i<3 && passtest;i++){
			int pick=rand()%(18-i);
			it=recList.begin();
			for(int j=0;j<pick;j++){
				it++;
			}

			rc=recList.erase(it++);
			removeItem(mirror,pick,18-i);
			if(rc!=it || !checkList(recList,mirror,18-i-1)){
				passtest=false;
				cout << "Error 6c:  Bug in erase." << endl;
			}
		}
		SortedList<int>::iterator it2=intList.begin(); 
		SortedList<int>::iterator rc2;
		rc2=intList.erase(it2++);
		removeItem(intMirror,0,cap);
		if(rc2!=it2 || !checkList(intList,intMirror,cap-1)){
			passtest=false;
			cout << "Error 6d: Bug in erase." << endl;
		}
		rc2=intList.erase(it2++);
		removeItem(intMirror,0,cap-1);
		if(rc2!=it2 || !checkList(intList,intMirror,cap-2)){
			passtest=false;
			cout << "Error 6e: Bug in erase." << endl;
		}
		it2=intList.begin();
		for(int i=0;i<cap-4;i++){
			it2++;
		}
		SortedList<int>::iterator secondLast=it2;
		it2++;
		rc2=intList.erase(it2++);
		removeItem(intMirror,cap-3,cap-2);
		if(rc2!=intList.end() || !checkList(intList,intMirror,cap-3)){
			passtest=false;
			cout << "Error 6f: Bug in remove." << endl;
		}
		rc2=intList.erase(secondLast);
		removeItem(intMirror,cap-4,cap-3);
		if(rc2!=intList.end() || !checkList(intList,intMirror,cap-4)){
			passtest=false;
			cout << "Error 6g: Bug in remove." << endl;
		}
		for(int i=0;i<100 && passtest;i++){
			int pick=rand()%(cap-i-4);

			it2=intList.begin();
			for(int j=0;j<pick;j++){
				it2++;
			}

			rc2=intList.erase(it2++);
			removeItem(intMirror,pick,cap-4-i);
			if(rc2!=it2 || !checkList(intList,intMirror,cap-i-5)){
				passtest=false;
				cout << "Error 6h:  Bug in remove." << endl;
			}
		}
	}
	if(passtest){
		numPassed++;
		cout << "test 7: ensure that copied list were not altered (proper deep copy was made) " << endl;
		if(!checkList(recCopy,data,20)){
			passtest=false;
			cout << "Error 7a: Bug in copy constructor, deep copy not made?"  << endl;
			#if VERBOSE >= 1
			printLists(recCopy,data,20);
			#endif
		}
		if(!checkList(intCopy,intData,cap)){
			passtest=false;
			cout << "Error 7b: Bug in copy constructor, deep copy not made?" << endl;
			#if VERBOSE >= 2
			printLists(intCopy,intData,cap);
			#endif
		}
	}



	if(passtest){
		numPassed++;
		cout << "test 8: test assignment operator"  << endl;
		recCopy = recList;
		intCopy = intList;
		if(!checkList(recCopy,mirror,15)){
			passtest=false;
			cout << "Error 8a: Bug in = operator"  << endl;  		
			#if VERBOSE >= 1
			printLists(recCopy,mirror,15);
			#endif
		}
		if(!checkList(intCopy, intMirror,cap-104)){
			passtest=false;
			cout << "Error 8b: Bug in = operator"  << endl;  		
			#if VERBOSE >= 2
			printLists(intCopy,intMirror,cap-104);
			#endif
		}
		duplicateArray(data,mirror,20);
		duplicateArray(intData,intMirror,cap);
	}


	if(passtest){
		numPassed++;
		cout << "test 9: test erase(from,to) function"  << endl;
		SortedList<Record>::iterator from=recList.begin();
		SortedList<Record>::iterator to=from;
		SortedList<Record>::iterator rc;
		for(int i=0;i<3;i++){
			to++;
		}
		rc=recList.erase(from,to);
		removeRange(mirror,0,3,15);
		if(rc!=to || !checkList(recList,mirror,12)){
			passtest=false;
			cout << "Error 9a: Bug in erase(from,to).  removed first 3 items" << endl;
			#if VERBOSE >= 1
			printLists(recList,mirror,12);
			#endif
		}
		from=recList.begin();
		for(int i=0;i<9;i++){
			from++;
		}
		rc=recList.erase(from,recList.end());
		removeRange(mirror,9,12,12);
		if(rc!=recList.end() || !checkList(recList,mirror,9)){
			passtest=false;
			cout << "Error 9b: Bug in erase(from,to).  removed last 3 items" << endl;
			#if VERBOSE >= 1
			printLists(recList,mirror,9);
			#endif			
		}
		from=recList.begin();
		for(int i=0;i<3;i++){
			from++;
		}
		to=from;
		for(int i=0;i<3;i++){
			to++;
		}
		rc=recList.erase(from,to);
		removeRange(mirror,3,6,9);
		if(rc!=to || !checkList(recList,mirror,6)){
			passtest=false;
			cout << "Error 9c: Bug in erase(from,to).  removed 4th,5th and 6th item" << endl;
			#if VERBOSE >= 1
			printLists(recList,mirror,6);
			#endif			
		}

	}

	if(passtest){
		numPassed++;
		cout << "test 10: test assignment operator (deep copy)"  << endl;
		if(!checkList(recCopy,data,15)){
			passtest=false;
			cout << "Error 10a: Bug in = operator, no deepcopy?"  << endl;
		}
		if(!checkList(intCopy,intData,cap-104)){
			passtest=false;
			cout << "Error 10b: Bug in = operator,no deep copy?"  << endl;     
		}
	}
	if(passtest){
		numPassed++;
	}
	if(numPassed == 10){
		cout << "Testing for Assignment 1 completed successfully" << endl;
	}
	else{
		cout << numPassed << " / 10 tests passed.  Looks like you still" << endl;
		cout << "have some work to do" << endl;
	}
	return 0;
}

template <class T>
void removeItem(T arr[],int idx,int sz){
	for(int i=idx;i<sz-1;i++){
		arr[i]=arr[i+1];
	}
}
template <class T>
void removeRange(T arr[],int from,int to, int sz){
	int diff=to-from;
	for(int i=from;i<sz-diff;i++){
		arr[i]=arr[i+diff];
	}
}

template <class T>
bool checkList(SortedList<T>& list,const T array[],int sz){
	bool rc=true;
	auto it=list.begin();
	int i;
	for(i=0;i<sz-1 && it!=list.end() && rc;i++,it++){
		if(*it != array[i]){
			rc=false;
		}
	}
	if(*it != array[i]){
		rc=false;
	}
	for(i=sz-1;i>0 && it!=list.begin() && rc; i--, it-- ){
		if(*it != array[i]){
			rc=false;
		}

	}
	if(*it != array[i]){
		rc=false;
	}
	for(i=0;i<sz && it!=list.end() && rc;i++,it++){
		if(*it != array[i]){
			rc=false;
		}
	}

	if(it!=list.end() || i < sz){
		rc=false;
	}
	return rc;
}

template <class T>
void duplicateArray(T dest[], const T src[],int sz){
	for(int i=0;i<sz;i++){
		dest[i]=src[i];
	}
}

template <class T>
void printLists(SortedList<T>& list,const T array[],int sz){
	auto it=list.begin();
	for(int i=0;i<sz;i++,it++){
		cout << "correct: "  << array[i] << " your list: " << *it << endl;

	}
}

template <class T>
void mergeSort(T arr[],T tmp[],int start,int end){
  //if the array is more than one element big
	if(start<end){
		int mid=(start+end)/2;
		mergeSort(arr,tmp,start,mid);
		mergeSort(arr,tmp,mid+1,end);
		mergeArray(arr,tmp,start,mid+1,end);
	}
}
/*function merges the two halfs of a sorted array together.
  The arrays are defined from arr[startA]to arr[startB-1] and arr[startB]
to arr[endB]*/

template <class T>
void mergeArray(T arr[],T tmp[],int startA,int startB,int endB){
	int aptr=startA;
	int bptr=startB;
	int idx=startA;
	while(aptr < startB && bptr < endB+1){
		if(arr[aptr] < arr[bptr]){
			tmp[idx++]=arr[aptr];
			aptr++;
		}
		else{
			tmp[idx++]=arr[bptr];
			bptr++;
		}
	}
//merge function only does one of the two following loops
	while(aptr<startB){
		tmp[idx]=arr[aptr];
		idx++;
		aptr++;    
	}
	while(bptr < endB+1){
		tmp[idx++]=arr[bptr];
		bptr++;
	}

//copy back into arr
	for(int i=startA;i<=endB;i++){
		arr[i]=tmp[i];
	}
}

/*this is the wrapper function that "main" uses.  The world see's this and
  use this. The actual work is done by the other MergeSort() function*/
template <class T>
void mergeSort(T arr[],int size){
	T* tmp=new T[size];
	mergeSort(arr,tmp,0,size-1);
	delete [] tmp;
}
