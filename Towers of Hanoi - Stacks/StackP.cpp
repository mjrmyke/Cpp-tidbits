// *********************************************************
// Implementation file StackP.cpp for the ADT stack.
// Pointer-based implementation.
// *********************************************************
#include "StackP.h" // header file
#include <stddef.h> // for NULL
#include <assert.h> // for assert

struct stackNode
{ stackItemType Item;
    ptrType Next;
}; // end struct

stackClass::stackClass() : TopPtr(NULL)
{
} // end default constructor

stackClass::stackClass(const stackClass& S)
{
    if (S.TopPtr == NULL)
        TopPtr = NULL; // original list is empty

    else
    { // copy first node
        TopPtr = new stackNode;
        assert(TopPtr != NULL);
        TopPtr->Item = S.TopPtr->Item;

        // copy rest of list
        ptrType NewPtr = TopPtr; // new list pointer
        for (ptrType OrigPtr = S.TopPtr->Next;OrigPtr != NULL;OrigPtr = OrigPtr->Next)
        { NewPtr->Next = new stackNode;
            assert(NewPtr->Next != NULL);
            NewPtr = NewPtr->Next;
            NewPtr->Item = OrigPtr->Item;
        } // end for
        NewPtr->Next = NULL;
    } // end if
} // end copy constructor

stackClass::~stackClass()
{
    bool Success;

// pop until stack is empty (Success is false)
    Pop(Success);
    while (Success)
        Pop(Success);
// Assertion: TopPtr == NULL
} // end destructor

bool stackClass::StackIsEmpty() const
{
    return bool(TopPtr == NULL);
} // end StackIsEmpty

void stackClass::Push(stackItemType NewItem, bool& Success)
{
// create a new node
    ptrType NewPtr = new stackNode;

    Success = bool(NewPtr != NULL); // check allocation
    if (Success)
    { // allocation successful; set data portion of new node
        NewPtr->Item = NewItem;

        // insert the new node
        NewPtr->Next = TopPtr;
        TopPtr = NewPtr;
    } // end if
} // end Push

void stackClass::Pop(bool& Success)
{
    Success = bool(not StackIsEmpty());
    if (Success)
    { // stack is not empty; delete top
        ptrType Temp = TopPtr;
        TopPtr = TopPtr->Next;

        // return deleted node to system
        Temp->Next = NULL; // safeguard
        delete Temp;
    } // end if
} // end Pop

void stackClass::Pop(stackItemType& StackTop, bool& Success)
{
    Success = bool(not StackIsEmpty());
    if (Success)
    { // stack is not empty; retrieve and delete top
        StackTop = TopPtr->Item;
        ptrType Temp = TopPtr;
        TopPtr = TopPtr->Next;

// return deleted node to system
        Temp->Next = NULL; // safeguard
        delete Temp;
    } // end if
} // end Pop

void stackClass::GetStackTop(stackItemType& StackTop,
        bool& Success) const
{
    Success = bool(not StackIsEmpty());
    if (Success)
        // stack is not empty; retrieve top
        StackTop = TopPtr->Item;
} // end GetStackTop
// End of implementation file.