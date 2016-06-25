
//singly linked list
template <typename T>
class SList{
	struct Node{
		T data_;
		Node* next_;
		Node(const T& data=T{}, Node* next=nullptr){
			data_=data;
			next_=next;
		}
	};
	Node* first_;
	Node* last_;
public:
	class iterator{
		friend class SList;
		Node* curr_;
		SList* myList_;
		iterator(Node* curr,SList* myList){
			curr_=curr;
			myList_=myList;
		}
	public:
		iterator(){
			curr_=nullptr;
			myList_=nullptr;
		}
		T& operator*(){
			return curr_->data_;
		}
		const T& operator*() const{
			return curr_->data_;
		}
		iterator operator++(){
			//++x
			if(curr_){
				curr_=curr_->next_;
			}
			return *this;
		}
		iterator operator++(int){
			//x++
			iterator old=*this;
			if(curr_){
				curr_=curr_->next_;
			}
			return old;
		}
		bool operator==(const iterator& other){
			return other.curr_==curr_;
		}
		bool operator!=(const iterator& other){
			return other.curr_!=curr_;
		}
	};
	//creates empty linked list
	SList(){
		first_=last_=nullptr;
	}
	//insert node at front of list and return iterator
	//to that node
	//O(1)
	iterator insertFront(const T& data){
		Node* nn=new Node(data,first_);
		first_=nn;
		if(first_->next_==nullptr){
			last_=nn;
		}
		iterator rc(nn,this);
		return rc;
	}
	//inserts a node to end of list
	//and returns iterator to newly added node
	//O(1)
	iterator insertBack(const T& data){
		Node* nn=new Node(data,nullptr);
		if(last_==nullptr){
			//empty list
			first_=nn;
		}
		else{
			last_->next_=nn;
	
		}
		last_=nn;
		iterator rc(nn,this);
		return rc;
	}

	//inserts a node after the node it refers to
	//and returns iterator to newly added node
	//O(1)
	iterator insert(iterator& it, const T& data){
		iterator rc;
		if(it.myList_ == this && it!=end()){
			Node* curr=it.curr_;
			Node* nn=new Node(data,curr->next_);
			curr->next_=nn;
			if(curr==last_){
				last_=nn;
			}
			rc=iterator(nn,this);
		}
		return rc;

	}

	//O(1)
	void removeFront(){
		if(first_){
			Node* tmp=first_;
			first_=first_->next_;
			if(first_==nullptr){
				last_=nullptr;

			}
			delete tmp;
		}

	}
	//O(n)
	void remove(iterator& it){
		if(it!= end() && it.myList_ == this){
			Node* rm = it.curr_;
			if(rm == first_){
				removeFront();
				/*
				first_=first_->next_;
				if(first_==nullptr){
					last_=nullptr;
				}
				delete rm;
				*/
			}
			else{
				Node* curr=first_;
				while(curr->next_!=rm){
					curr=curr->next_;
				}
				curr->next_=rm->next_;
				if(rm == last_){
					last_=curr;
				}
				delete rm;
			}

		}
	}

	//O(n)
	void removeBack(){
		if(last_){
			Node* rm =last_;
			if(first_==last_){
				delete rm;
				first_=last_=nullptr;				
			}
			else{
				Node* curr=first_;
				//make curr point to second last node
				while(curr->next_!=rm){
					curr=curr->next_;
				}
				curr->next_=nullptr;
				last_=curr;
				delete rm;
			}
		}

	}
	iterator begin(){return iterator(first_,this);}
	iterator end(){return iterator(nullptr,this);}
};