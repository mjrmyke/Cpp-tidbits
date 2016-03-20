// *********************************************************
// Header file StackP.h for the ADT stack.
// Pointer-based implementation.
// *********************************************************
struct node
{int n;
    int number;
    char source;
    char end;
    char spare;

};
typedef node stackItemType;
struct stackNode;
typedef stackNode* ptrType;
class stackClass
{
public:
// constructors and destructor:
    stackClass();                     // default constructor
    stackClass(const stackClass& S);// copy constructor
    ~stackClass();                    // destructor
// stack operations:

    bool StackIsEmpty() const;
// Determines whether a stack is empty.
    void Push(stackItemType NewItem, bool& Success);
// Adds an item to the top of a stack.
    void Pop(bool& Success);
// Removes the top of a stack.
    void Pop(stackItemType& StackTop, bool& Success);
// Retrieves and removes the top of a stack.
    void GetStackTop(stackItemType& StackTop, bool& Success) const;
// Retrieves the top of a stack.
private:
    ptrType TopPtr;
}; // end class
