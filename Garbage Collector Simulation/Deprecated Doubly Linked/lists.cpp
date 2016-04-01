/* Myke Walker
File: lists.cpp
*/

#include"List.h"
#include<cstdlib>
#include<iostream>

using namespace std;

//Class Link
template<class E>
class Link
{
private: // static to make available to all links in list
    static Link<E>*	freeList;
    static int	Linkcount;
    static int	freelinkcnt;
public:
    mark bool;
    E data;
    Link<E>* prev;
    Link<E>* next;


	Link(){  //constructor responsible for keeping track of # of nodes
		Linkcount++;
		if (freelinkcnt > 0){
			freelinkcnt--;
		}
	}

    Link(E newData, Link<E>* newPrev = 0, Link<E>* newNext = 0) : data(newData), prev(newPrev), next(newNext){//constructor responsible for keeping track of # of nodes
        Linkcount++;
        if (freelinkcnt > 0){
            freelinkcnt--;
        }
    }

    //overloaded ops
    void* operator new(size_t size);
    void operator delete(void* p);

    //make data members retrievable
    Link<E>* getFreeList();
    int getLinkcount();
    int getfreelinkcnt();
}; //end class link


template<class E>
Link<E>* Link<E>::freeList = 0;

template<class E>
int Link<E>::Linkcount = 0;

template<class E>
int Link<E>::freelinkcnt = 0;

template<class E>
void* Link<E>::operator new(size_t size){
    if (!freeList){	//if no freelist, gen link normally
        return ::new char[sizeof(Link<E>)];
    }
    else{
    Link<E>* newLink = freeList; //freelist found, steal head of freelist
    freeList = freeList->next;
    return newLink;
  }
}//end overloaded new


template<class E>
void Link<E>::operator delete(void* del){ //overloaded del


    Link<E>* linkToDelete = static_cast<Link<E>*> (del); // have to cast it? dunno why. pointer to Link
    linkToDelete->next = 0;
    linkToDelete->prev = 0;//set to zero so we dont have pointers floating in mem

    //keep track of size of lists
    freelinkcnt++;
    Linkcount--;

    if (!freeList){ // if freelist is empty, make this node the head of the freelist
        freeList = linkToDelete;
        return;
    }
    else{
      freeList->prev = linkToDelete;
      linkToDelete->next = freeList;
      freeList = linkToDelete;
    }
}

template<class E>//gets freelist
Link<E>* Link<E>::getFreeList(){return freeList;}

template<class E>//gets count of links in list
int Link<E>::getLinkcount(){return Linkcount;}

template<class E>//return number of links in the freelist
int Link<E>::getfreelinkcnt(){return freelinkcnt;}

//--------------------------------------------------------------
//Double Linked List Class - extended from LList class from book
//--------------------------------------------------------------

template<class E>
class DLList : public List<E>
{
private:
    Link<E>*	currNode;
    Link<E>*	head;
    Link<E>*	tail;
    void delhead();//Added to handle clear function
public:
    DLList(Link<E>* tempNode = 0, Link<E>* tempHead = 0, Link<E>* tempTail = 0) : currNode(tempNode), head(tempHead), tail(tempTail) {}
    ~DLList();
    void clear();
    void prepend(const E& item);
    void append(const E& item);
    void insert(const E& item);
    bool moveToStart();
    bool moveToEnd();
    bool next();
    bool prev();
    const E* getValue() const;
    int numActive();
    int numFree();

};//end Class Header

template<class E>
int DLList<E>::numActive(){return head->getLinkcount();}

template<class E>
int DLList<E>::numFree(){return head->getfreelinkcnt();}

template<class E>//Destructor
DLList<E>::~DLList(){
    while (head != 0){
        Link<E>* oldHead = head;
        head = head->next;
        delete oldHead;
    }
}

template<class E>//Delete head node function for clear
void DLList<E>::delhead(){
    if (!head || head == tail){
        delete head;
        head = 0;
        tail = 0;
        currNode = 0;
        return;
    }
    Link<E>* temp = head;
    head = head->next;
    delete temp;
}

template<class E>//Clear function, uses delhead
void DLList<E>::clear(){
    while (head != 0){
        delhead();
    }
}

template<class E>//Prepend function, modified append to prepend
void DLList<E>::prepend(const E& item){
	Link<E>* newLink = new Link<E>(item);
    if (head == 0){
		  head = newLink;
		  tail = newLink;
		  currNode = newLink;
      return;
    }
    head->prev = newLink;
    newLink->next = head;
	  head = newLink;
}

template<class E>//added prev handling from book function
void DLList<E>::append(const E& item){
    Link<E>* newLink = new Link<E>(item);
    if (head == 0){
        head = newLink;
        tail = newLink;
        currNode = newLink;
        return;
    }
    newLink->prev = tail;
    tail->next = newLink;
    tail = newLink;
}

template<class E>//added prev handling from book function
void DLList<E>::insert(const E& item){
    Link<E>* newLink = new Link<E>(item);
    if (head == 0 || head->data >= item){
        head = newLink;
        // tail = newLink;
        currNode = newLink;
        return;
    }
    else{
      currNode = head;
      while (current->next != NULL && current->next->data < item) {currNode = currNode->next;}
      newlink->next = current->next;
      current->next = newlink;
    }
//insertion sort goes here
}


template<class E>//same as book
bool DLList<E>::moveToEnd(){
    if (head == 0){return false;}
    currNode = tail;
    return true;
}

template<class E>//same as book
bool DLList<E>::moveToStart(){
  if (head == 0){return false;}
  currNode = head;
  return true;
}

template<class E>//modified next to change to prev
bool DLList<E>::prev(){
    if (head == 0 || currNode->prev == 0){return false;}
    currNode = currNode->prev;
    return true;
}

template<class E>//same as book
bool DLList<E>::next(){
  if (head == 0 || currNode->next == 0){return false;}
  currNode = currNode->next;
  return true;
}

template<class E>//tried to modify to print prev and next
const E* DLList<E>::getValue() const{
  // E* tempprev = &(currNode->prev);
  // E* tempnext = &(currNode->next);
  // cout << "Previous Value is: " <<tempprev<<"\n";
  // cout << "Next Value is: "<<tempnext<<"\n";
	const E* value = &(currNode->data);
  return value;
}
