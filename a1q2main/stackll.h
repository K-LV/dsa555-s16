#include "slist.h"
template <typename T>
class Stack{
	SList<T> data_;
public:
	Stack(){
	}
	void push(const T& data){
		data_.insertFront(data);
	}
	void pop(){
		data_.removeFront();
	}
	T top(){
		return *(data_.begin());
	}
	bool isEmpty(){
		return (data_.begin()==data_.end());
	}
};