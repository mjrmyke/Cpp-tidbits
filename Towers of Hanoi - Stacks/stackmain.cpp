#include "StackP.h"
#include <iostream>
using namespace std;
//Myke W

void TowerSolver(int number, char source, char end, char spare);

int main()
{
    int count;
    cout<<"How many rings high is the tower?\n";
    cin>>count;
    TowerSolver(count,'a', 'b', 'c');
}

void TowerSolver(int number, char source, char end, char spare){
    stackClass S;
    stackItemType top,NewNode;
    bool success;

//beginning values
    top.number = number;
    top.source = source;
    top.end = end;
    top.spare = spare;

    S.Push(top,success);

    while(not S.StackIsEmpty())
    {
        S.Pop(top,success);

        if (top.number == 1){
            cout<<"Move " <<top.source<<" to "<<top.end<<"."<<endl;
        }
        else {

            NewNode.number = top.number - 1;
            NewNode.source = top.spare;
            NewNode.end = top.end;
            NewNode.spare = top.source;
            S.Push(NewNode, success);

            NewNode.number = 1;
            NewNode.source = top.source;
            NewNode.end = top.end;
            NewNode.spare = top.spare;
            S.Push(NewNode, success);

            NewNode.number = top.number - 1;
            NewNode.source = top.source;
            NewNode.end = top.spare;
            NewNode.spare = top.end;
            S.Push(NewNode, success);


        }
    }

}
