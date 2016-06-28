#include "maze.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#define VERBOSE 0
//this is not the real tester
//compile this with your program:
//g++ maze.cpp a1q2.cpp mazetest.cpp;
int findPath (Maze& theMaze, const Coord& start, const Coord& end, Coord path[]);
void printPath(const char* fname, Coord path[],int n);
void printPath2(const char* fname, Coord path[],int n);
int readCorrectPath(const char* fname, Coord path[]);
bool operator==(const Maze& left, const Maze& right);
bool comparePath(Coord mazePath[], Coord correctPath[], int n);
bool testAlternativePath(const Maze* original,const Maze* marked, Coord start, Coord end, const Coord path[], int n);

int main(void){
	int testStart[7][2]={{1,1},{75,69},{75,69},{99,85},{1,1},{1,1}};
	int testEnd[7][2]={{1,99},{99,85},{75,69},{75,69},{191,199},{75,69}};
	const char* mazeFiles[10]={"maze1.txt", "maze1.txt","maze1.txt", "maze1.txt","maze2.txt", "maze3.txt"};
	const char* pathFiles[10]={ "test1path.txt","test2path.txt","test3path.txt","test4path.txt", "test5path.txt","test6path.txt"};
	const char* correctFiles[10]={"correct1.txt","correct2.txt","correct3.txt","correct4.txt","correct5.txt","correct6.txt"};
	const char* json[10]={"test1result.txt","test2result.txt","test3result.txt","test4result.txt","test5result.txt","test6result.txt"};
	const int numTests=6;
	const int correctLengths[10]={803,733,1,733, 5641,0};
	int pathLength;
	Coord start;
	Coord end;
	Coord path[10000];
	Coord correctPath[10000];
	Maze* m;
	Maze* correctMaze;
	Maze* original;
	ofstream out;
	int numPassed=0;
	for(int i=0;i<numTests&& i == numPassed;i++){
		m=new Maze(mazeFiles[i]);
		readCorrectPath(pathFiles[i],correctPath);

		start.set(testStart[i][0],testStart[i][1]);
		end.set(testEnd[i][0],testEnd[i][1]);
		pathLength=findPath (*m, start, end, path);
		correctMaze=new Maze(correctFiles[i]);

		if(pathLength!=correctLengths[i]){
			original=new Maze(mazeFiles[i]);
			cout << "checking alternate path for Test #" << i+1 << endl;
			if(testAlternativePath(original, m, start, end, path, pathLength)){
				cout << "Test # " << i+1 << " passed" << endl;			
				numPassed++;
			}
			else{
				cout << "Test # " << i+1 << " failed" << endl;			
			}

		}
		else{
			if(*m == *correctMaze){
				if(comparePath(correctPath, path,correctLengths[i])){
					cout << "Test # " << i+1 << " passed" << endl;
					numPassed++;

				}
				else{
					cout << "Test # " << i+1 << " failed" << endl;
					cout << "Your maze was correctly marked but the path is not correct" << endl;

				}
			}
			else{
				cout << "Test # " << i+1 << " failed" << endl;
				cout << "Your maze was not the same as the marked maze." << endl;
				cout << "Make sure you have an updated version of the maze.h/maze.cpp files?" << endl;
			}
		}
		if(pathLength > 0){
			printPath(json[i],path,pathLength);
		}
		#if VERBOSE == 1
		cout << "your maze" << endl;
		m->print();
		#endif
		delete correctMaze;
		delete m;
	}
}
bool operator==(const Maze& left, const Maze& right){
	bool rc=true;
	if(left.height() != right.height() || left.width() != right.width()){
		rc=false;
	}
	else{
		Coord l;
		Coord r;
		for (int i=0;i < left.height() && rc;i++){
			for(int j=0;j<right.width() && rc;j++){
				l.set(j,i);
				r.set(j,i);
				if( !((left.isWall(l) && right.isWall(r))|| 
					(left.isEmpty(l) && right.isEmpty(r)) ||
					(left.isMarked(l) && right.isMarked(r)))){
					rc=false;
				}
			}
		}
	}
	return rc;
}
bool operator==(const Coord& left, const Coord& right){
	bool rc=false;
	if(left.x == right.x && left.y == right.y){
		rc=true;
	}
	return rc;
}
bool operator!=(const Coord& left, const Coord& right){
	bool rc=false;
	if(left.x != right.x || left.y != right.y){
		rc=true;
	}
	return rc;
}
void printPath(const char* fname, Coord path[],int n){
	ofstream fs(fname);
	fs << "[";
	for(int i=0;i<n-1;i++){
		fs << path[i].x << ", " << path[i].y <<", " ;
	}
	fs << path[n-1].x << ", " << path[n-1].y <<"]" << endl;
	fs.close();
}
int readCorrectPath(const char* fname, Coord path[]){
	ifstream fs(fname);
	int rc;
	int x;
	 int y;
	fs >> rc;
	for(int i=0;i<rc;i++){
		fs >> x >> y;
		path[i].set(x,y);
	}
	return rc;
}
bool comparePath(Coord mazePath[], Coord correctPath[], int n){
	bool rc=true;
	for(int i=0;i<n && rc;i++){
		if(mazePath[i].x!=correctPath[i].x || mazePath[i].y != correctPath[i].y){
			rc=false;
		}
	}
	return rc;
}
bool isBeside(const Coord& a, const Coord& b){
	bool rc=true;
	if(a.x == b.x){
		if(abs(a.y-b.y) != 1){
			rc=false;
		}
	}
	else if(a.y==b.y){
		if(abs(a.x-b.x) != 1){
			rc=false;
		}
	}
	else{
		rc=false;
	}
	return rc;
}
bool search(const Coord& c, const Coord path[], int n){
	bool found=false;
	for(int i=0;!found && i<n;i++){
		if(c==path[i]){
			found=true;
		}
	}
	return found;
}
bool testAlternativePath(const Maze* original,const Maze* marked, Coord start, Coord end, const Coord path[], int n){
	bool isGood=true;
	if(path[0]!= start){
		isGood=false;
	}
	if(path[n-1]!= end){
		isGood=false;
	}
	for(int i=1;isGood && i<n;i++){
		if(!isBeside(path[i],path[i-1])){
			isGood=false;
		}
		if(!original->isEmpty(path[i])){
			isGood=false;
		}
		if(!marked->isMarked(path[i])){
			isGood=false;
		}
	}
	Coord curr;
	for(int i=0;isGood && i<original->height();i++){
		for(int j=0;isGood && j<original->width();j++){
			curr.set(j,i);
			if(marked->isMarked(curr)){
				if(!search(curr,path,n)){
					isGood = false;
				}
			}
		}
	}
	return isGood;
}