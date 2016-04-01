/* Myke Walker Lab 4 csci 115
File: driver.cpp
*/

#include<iostream>
#include"List.h"
#include"lists.cpp"

bool done, success;
char choice, input;
int index;

template<class E>
class List;

template<class E>
class Link;

template<class E>
class DLList;

int main()
{
    using namespace std;

    DLList<char> DoubleLink[10];



    while (not done)
        {
        //print menu
        cout<<"\n\nA --(A)ctive number of Links\n"
        <<"C --(C)lear list\n"
        <<"F --(F)reeList Available Links\n"
        <<"G --(G)o to Start of List\n"
        <<"L --(L)ist all Nodes\n"
        <<"M --(M)ove to end of List\n"
        <<"N --Go to (N)ext item in List\n"
        <<"P --Go to(P)revious item in list\n"
        <<"R --P(r)epend to list\n"
        <<"S --(S)ubjoin [append] to list\n"
        <<"V --Get (V)alue at Current Position\n"
        <<"Q --(Q)uit\n";

        //get choice
        cout << endl;
        cout << endl;
        cout<<"Enter the letter indicating the Desired operation"<<endl;
        cin>>choice;
        //execute command
        switch (choice)
            {
            case 'a':
            case 'A':
                cout << "number of active links: " <<DoubleLink.numActive() << endl;
                break;
            case 'c':
            case 'C':
                DoubleLink.clear();
                cout <<"Cleared!"<<endl;
                break;
            case 'f':
            case 'F':
                cout << "number of avaialble links in freelist: " << DoubleLink.numFree() << endl;
                break;
            case 'g':
            case 'G':
                success = DoubleLink.moveToStart();
                if (!success){
                  cout << "operation was not successful\n";
                }
                break;
            case 'l':
            case 'L':
                success = DoubleLink.moveToStart();
                if (!success){
                  cout << "operation was not successful, possibly no nodes in list\n";
                }
                if (success){
                  cout <<"Listing all nodes."<<endl;
                  for (int i=0; i<DoubleLink.numActive();i++){
                    cout <<"node: "<<i<<" with value: "<< DoubleLink.getValue() <<endl;
                    success = DoubleLink.next();

                  }

                }

                break;
            case 'm':
            case 'M':
                success = DoubleLink.moveToEnd();
                if (!success){
                  cout << "operation was not successful\n";
                }
                break;
            case 'n':
            case 'N':
                success = DoubleLink.next();
                if (!success){
                  cout << "operation was not successful\n";
                }
                break;
            case 'p':
            case 'P':
                success = DoubleLink.prev();
                if (!success){
                  cout << "operation was not successful\n";
                }
                break;
            case 'r':
            case 'R':
                cout << "what character would you like to Prepend?\n";
                cin >> input;
                cout << endl;
                DoubleLink.prepend(input);
                break;

            case 's':
            case 'S':
                cout << "what character would you like to Append?\n";
                cin >> input;
                cout << endl;
                DoubleLink.append(input);
                break;
            case 'v':
            case 'V':
                cout <<"value: "<< DoubleLink.getValue()<<endl;
                break;
            case 'Q':
            case 'q':
                done=true;
                break;
            }
        }


   return 0;
}
