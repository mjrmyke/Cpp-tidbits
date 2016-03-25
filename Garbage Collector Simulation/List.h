/* Myke Walker
File: list.h
*/
#ifndef	LIST_H
#define	LIST_H

template<typename E> class List
{
private:
    void operator =(const List&) {} // protect assignment
    List(const List&) {}		//protect copy constructor
public:
    List() {}		//default constructor
    virtual ~List() {}	//base destrutor

    //clear contents from the list, freeing memory
    virtual void clear() = 0;

    //insert an element at the beginning of the list
    virtual void prepend(const E& item) = 0;

    //append an element at the end of the list
    virtual void append(const E& item) = 0;

    //set the current position to the first element of the list, if possible
    //return true if succesful, false if list was empty
    virtual bool moveToStart() = 0;

    //sset the current position to the last element of the list, if possible
    //return true if succesful, false if list was empty
    virtual bool moveToEnd() = 0;

    //move the current position one step right, if possible
    //return true if succesful, false if already at the end
    virtual bool next() = 0;

    //move the current position one step to the left if possible
    //return true if succesful, false if already at the beginning
    virtual bool prev() = 0;

    //return a pointer to the current element(or null if none)
    virtual const E* getValue() const = 0;

    //return total number of active nodes
    virtual int numActive() = 0;

    //return total number of free nodes
    virtual int numFree() = 0;

};

#endif
