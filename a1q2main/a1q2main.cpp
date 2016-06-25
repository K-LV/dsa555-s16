/*********************************************************************/
/*                                                                   */
/* Tester for DSA555-s16 assignment 1, question 2                    */
/* Version 1.0                                                       */
/* copy your file a1q2 to this folder.  There are many data files    */
/* and the maze.cpp/maze.h files had to be updated to enable some    */
/* further testing.  The update should have no effect on your code   */
/*                                                                   */
/* To compile:                                                       */
/*                                                                   */
/*      g++ a1q2main.cpp maze.cpp a1q2.cpp -std=c++0x                */
/*                                                                   */
/* Aside from indicating if you passed the 6 tests, it will also     */
/* produce a .js file.  In a day or so, you will be able to check    */
/* the running of your maze by loading the path file                 */
/*                                                                   */
/* detals to follow                                                  */
/*********************************************************************/

#include "maze.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#define VERBOSE 0
int findPath (Maze& theMaze, const Coord& start, const Coord& end, Coord path[]);
void printPath(const char* fname, Coord path[],int n);
void printPath2(const char* fname, Coord path[],int n);
int readCorrectPath(const char* fname, Coord path[]);
bool operator==(const Maze& left, const Maze& right);
bool comparePath(Coord mazePath[], Coord correctPath[], int n);

int main(void){
	int testStart[7][2]={{1,1},{75,69},{75,69},{99,85},{1,1},{1,1}};
	int testEnd[7][2]={{1,99},{99,85},{75,69},{75,69},{191,199},{75,69}};
	const char* mazeFiles[10]={"maze1.txt", "maze1.txt","maze1.txt", "maze1.txt","maze2.txt", "maze3.txt"};
	const char* pathFiles[10]={ "test1path.txt","test2path.txt","test3path.txt","test4path.txt", "test5path.txt","test6path.txt"};
	const char* correctFiles[10]={"correct1.txt","correct2.txt","correct3.txt","correct4.txt","correct5.txt","correct6.txt"};
	const char* json[10]={"test1.js","test2.js","test3.js","test4.js","test5.js","test6.js"};
	const int numTests=6;
	const int correctLengths[10]={803,733,1,733, 5641,0};
	int pathLength;
	Coord start;
	Coord end;
	Coord path[10000];
	Coord correctPath[10000];
	Maze* m;
	Maze* correctMaze;
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
			cout << "Test # " << i+1 << " failed" << endl;
			cout << "Your path is not the correct length." << endl;
			cout << "Your function returned: " << pathLength << endl;
			cout << "It should have been: " << correctLengths[i] << endl;
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