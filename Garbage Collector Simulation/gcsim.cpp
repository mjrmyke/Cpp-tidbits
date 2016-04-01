//Myke Walker GC Simulation

/* Program to insert in a sorted list */
#include<cstdlib>
#include<iostream>

using namespace std;

/* Link list node */
struct node
{
    int data;
    int mark;
    struct node* next;
};



/* function to insert a new_node in a list. Note that this
  function expects a pointer to head_ref as this can modify the
  head of the input linked list (similar to push())*/
void insert(struct node* head_ref, int new_data, struct node* freelist)
{
    struct node* current;
    struct node* tempnode;
    /* Special case for the head end */
    if (*head_ref == NULL)
    {
        tempnode = freelist;
        freelist = freelist->next;

    }
    else

        /* Locate the node before the point of insertion */
        current = head_ref;
        while (current->next!=NULL &&
               current->next->data < new_data)
        {
            current = current->next;
        }
        tempnode=freelist;      //grab node from freelist
        freelist=freelist->next;//move freelist down to next
        tempnode->next=current->next;//give new node its pointer to next item in ListNode

        current->next = tempnode;//give the item prior to this item the pointer to new item
        // freelist = freelist->next;

        current->data = new_data;//place data in node;

}



/* Function to print linked list */
void printList(struct node *head)
{
    struct node *temp = head;
    while(temp != NULL)
    {
        std::cout<< temp->data;
        temp = temp->next;
    }
}

/*function to search and delete node from list, then return to freelist*/
void delete(struct node *list, int todelete){
    struct node *temp = list;
    while (temp != NULL){
      if (temp->data == todelete){
        //delete and return to freelist
      }
      temp=temp->next;
    }
}

/*function garbage collect using the mark and sweep method*/
void garbagecollect (struct node *h1, struct node *h2, struct node *freelist){
int i = 0;

//mark
struct node *temp = h1;
while (temp->next != NULL){
  temp->mark = 1;
}

struct node *temp = h2;
while (temp->next != NULL){
  temp->mark = 1;
}

struct node *temp = freelist;
while (temp->next != NULL){
  temp->mark = 1;
}

//sweep
for (int k = 0; k<9;k++){
  if (arrayofnodes[k]->mark==1){
    arrayofnodes[k]->next = freelist;
    freelist = arrayofnodes[k];
  }
}

}


int main()
{

  struct node* arrayofnodes[9];
  struct node* h1= NULL;
  struct node* h2= NULL;
  struct node* freelist= arrayofnodes[0];

  //make sure nodes are connected
  arrayofnodes[0]->next = arrayofnodes[1];
  arrayofnodes[1]->next = arrayofnodes[2];
  arrayofnodes[2]->next = arrayofnodes[3];
  arrayofnodes[3]->next = arrayofnodes[4];
  arrayofnodes[4]->next = arrayofnodes[5];
  arrayofnodes[5]->next = arrayofnodes[6];
  arrayofnodes[6]->next = arrayofnodes[7];
  arrayofnodes[7]->next = arrayofnodes[8];
  arrayofnodes[8]->next = arrayofnodes[9];



  bool done;
  string choice,listchoice;


  while (not done){
    cout<<"\n\n(P)rint Memory\n"
    <<"(I)nsert"
    <<"(D)elete"
    <<"(G)arbageCollect"
    <<"(Q)uit\n";
    cout << endl;
    cout << endl;
    cout<<"Enter the letter indicating the Desired operation"<<endl;
    cin>>choice;

    switch (choice)
        {
        case 'p':
        case 'P':
          cout << "which list would you like printed, h1, h2, or freelist?\n";
          cin>>choice;
          printList(choice);
            break;
        case 'd':
        case 'D':
          cout<<"which list do you want to delete from?\n";
          cin>>listchoice;
          cout<<"what value do you want to delete\n";
          cin>>choice
          delete(listchoice,choice);
            break;
        case 'i':
        case 'I':
          cout<<"which list do you want to insert to?\n";
          cin>>listchoice;
          cout<<"what value do you want to insert\n";
          cin>>choice
          insert(listchoice,choice);
            break;
        case 'g':
        case 'G':
          garbagecollect(h1,h2,freelist);
            break;
        case 'Q':
        case 'q':
            done=true;
            break;
        }
  }





    return 0;
}
