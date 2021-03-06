//Myke Walker  Lab 6

#include "book.h"

#include <cstdio>
#include <cstring>
#include <fstream>

// Include definition for heaps
// We will use them to keep track of the forest of huffman trees during the
// building operation
#include "heap.h"

#define MAXCODELEN 20       // Max length of a huffman code
#define CODETABLELEN 100    // Maximum number of codes storable

using namespace std;
// CodeTable maps objects to their associated codes.
template <typename E>
class CodeTable {
private:
  E* obs;     // Objects
  char** codes;  // Associated code values
  int currsize;  // Current number of objects in table
  int maxsize;   // Max objects permitted in table
public:
  CodeTable(int size) {
    obs = new E[size];
    codes = new char*[size];
    for (int i = 0; i<size; i++) {
      codes[i] = new char[MAXCODELEN+1];
      for(int j=0; j<=MAXCODELEN; j++)
        codes[i][j] = '-';
	codes[i][MAXCODELEN] = '\0';
      }
    maxsize = size;  currsize = 0;
  }

  void addobject(E& obj) {
    Assert(currsize < maxsize, "CodeTable is full!");
    obs[currsize++] = obj;
  }

  char* getcode(E obj) {
    for (int i=0; i<currsize; i++)
      if(obj == obs[i]) return codes[i];
    return NULL;
  }
};

#include "huffnode.h"

template <typename E>
ostream& operator << (ostream& s, HuffNode<E>* z)
{
  if (z->isLeaf())
    return s << ((LeafNode<E>*)z)->val();
  else
    return s << z->weight();
}

#include "hufftree.h"

// Space for the heap's array
HuffTree<char>** TreeArray = NULL;

// Overload for the HuffTree << operator
template <typename E>
ostream& operator << (ostream& s, HuffTree<E>* z)
  { return s << z->weight(); }

// Comparator for the heap
class minTreeComp {
public:
  static bool prior(HuffTree<char>* x, HuffTree<char>* y)
    { return x->weight() < y->weight(); }
};

// Read the list of frequencies, make the forest, and set the
// list of entries into the code table.
int read_freqs(CodeTable<char>* ct, FILE* fp)
{ // Read a list of strings and frequencies from standard input,
  // building a list of Huffman coding tree nodes
  char buff[100];
  char buff2[100];
  char *ptr;
  char *ptr2;
  int freq;

  Assert(fgets(buff, 99, fp) != NULL,   // Read number of chars
           "Couldn't read character count");
  ptr = buff;
  Assert(isdigit(*ptr) != 0, "Must be a digit here.");
  int count = atoi(ptr);
  TreeArray = new HuffTree<char>*[count];
  for (int i=0; i<count; i++) { // Read in the frequencies
    Assert(fgets(buff, 99, fp) != NULL,
 	   "Ran out of codes too early");  // Read the next entry
    // process the entry, creating a new HuffTree
    for(ptr=buff; *ptr==' '; ptr++);  // Read first word
    Assert(*ptr == '"', "First char was not a quote mark.");
    for (ptr2=buff2,ptr++; *ptr!='"'; ptr++)
      *ptr2++ = *ptr;
    *ptr2 = '\0'; // End of string
    for (ptr++; *ptr==' '; ptr++);
    Assert(isdigit(*ptr) != 0, "Must be a digit here.");
    freq = atoi(ptr);
    ct->addobject(buff2[0]);
    TreeArray[i] = new HuffTree<char>(buff2[0], freq);
  }
  return count;
}

// Build a Huffman tree from a collection of frequencies
template <typename E> HuffTree<E>*
buildHuff(HuffTree<E>** TreeArray, int count) {
  heap<HuffTree<E>*,minTreeComp>* forest =
    new heap<HuffTree<E>*, minTreeComp>(TreeArray,
                                        count, count);
  HuffTree<char> *temp1, *temp2, *temp3 = NULL;
  while (forest->size() > 1) {
    temp1 = forest->removefirst();   // Pull first two trees
    temp2 = forest->removefirst();   //   off the list
    temp3 = new HuffTree<E>(temp1, temp2);
    forest->insert(temp3);  // Put the new tree back on list
    delete temp1;        // Must delete the remnants
    delete temp2;        //   of the trees we created
  }
  return temp3;
}


void decode(HuffTree<char>* theTree, char* code, char& msg, int& cnt) {
  HuffNode<char>* currnode = theTree->root();
  while (!currnode->isLeaf()) {
    cnt++;
    if (code[cnt-1] == '0') currnode = ((IntlNode<char>*)currnode)->left();
    else if (code[cnt-1] == '1') currnode = ((IntlNode<char>*)currnode)->right();
    else Assert(false, "Bad code character");
  }
  msg = ((LeafNode<char>*)currnode)->val();
}

void buildcode(HuffNode<char>* root, CodeTable<char>* ct,
	       char* prefix, int level, double& total) {
  if (root->isLeaf()) {
    cout << ((LeafNode<char>*)root)->val() << "\t" << prefix << "\n";
    strcpy(ct->getcode(((LeafNode<char>*)root)->val()), prefix);
    total += level * root->weight();
  }
  else {
    prefix[level] = '0';
    prefix[level+1] = '\0';
    buildcode(((IntlNode<char>*)root)->left(), ct, prefix, level+1, total);
    prefix[level] = '1';
    prefix[level+1] = '\0';
    buildcode(((IntlNode<char>*)root)->right(), ct, prefix, level+1, total);
    prefix[level] = '\0';
  }
}


void do_commands(HuffTree<char>* theTree,
                 CodeTable<char>* theTable, FILE *fp)
{
  int currchar;
  char buff[80];

  while (fgets(buff, 99, fp)) {
    if (strncmp(buff, "decode", 6) == 0) {
      for (currchar=0; buff[currchar] != '"'; currchar++);
      cout << "Decode " << &buff[currchar++];
      while (buff[currchar] != '"') {
        int cnt = 0;
        char msg;
        decode(theTree, &buff[currchar], msg, cnt);
        cout << msg << endl;
	currchar += cnt;
      }
    }
    else if(strncmp(buff, "encode", 6) == 0) {
      for (currchar=0; buff[currchar] != '"'; currchar++);
      cout << "Encode " << &buff[currchar++];
      for(; buff[currchar] != '"' ; currchar++) // Assume codes are characters.  Should generalize this.
        if (buff[currchar] == ' ') cout << ' ';
        else cout << theTable->getcode(buff[currchar]);
    }
    cout << "\n";
  }
}


// Driver for testing Huffman tree code
int main(int argc, char** argv) {
  // This will be the eventual Huffman tree
  HuffTree<char>* theTree;
  CodeTable<char>* theTable =
	   new CodeTable<char>(CODETABLELEN);
  // Working storage for the tree traversal that builds the code table
  char prefix[MAXCODELEN+1];
  // total is used to calculate the average code length
  double total = 0;
  FILE *fp;  // The file pointer
  bool done;

  ofstream myfile;
  string cintext;



while (not done){
  cout << "Choose your destiny\n"
       << "1 - Create tree from input File\n"
       << "2 - Create tree from cinning text\n"
       << "3 - Binary String to Decode\n"
       << "4 - Quit\n";
  cout<<"Enter the number indicating the Desired operation"<<endl;

  int a;
  int input, count;
  cin >> input;

  switch (input){
    case 1:
      // Check command line parameter for frequency file
      Assert(argc == 2, "Usage: huffman <frequency file>");
      Assert((fp = fopen(argv[1], "rt")) != NULL, "No such file");

      // Now, read in the list of frequencies, and initialize the
      //   forest of Huffman trees.
      cout << "Read frequencies\n";
      count = read_freqs(theTable, fp);

      //  forest->print();

      // Now, build the tree.
      cout << "Build the tree\n";
      theTree = buildHuff<char>(TreeArray, count);

      // Now, output the tree, which also creates the code table.
      cout << "Output the tree\n";
      buildcode(theTree->root(), theTable, prefix, 0, total);
      cout << "Average code length is "
           << total/(double)theTree->weight() << "\n";
      cout << "space used before Huff tree: " << total * 8<<endl;
      cout << "average space used after Huff tree: " << total/(double)theTree->weight() * 8<<endl;
      cout << "compression ratio = " << (total/(double)theTree->weight() * 8) / (total * 8)<<endl;

      // Finally, do the encode/decode commands to test the system.
      do_commands(theTree, theTable, fp);
      break;
    case 2:
      myfile.open("cintext.txt");
      cout << "Enter number of characters used in input" << endl;
      cin.get();
      getline(cin, cintext);
      myfile << cintext << endl;
      while (a != -1) {
        cout << "Enter in a character in quotes and a weight, or a -1 to continue" << endl;
        getline(cin, cintext);
        if (cintext != "-1"){myfile << cintext << endl;}
        else{a = -1;}
        }
        a=0;
      while (a != -1) {
        cout << "Enter your Encode or Decode action followed by the value in quotes, or -1 to continue" << endl;
        getline(cin, cintext);
        if (cintext != "-1"){myfile << cintext << endl;}
        else{a = -1;}
        }


      myfile.close();
      fp = fopen("cintext.txt", "rt");

      // Now, read in the list of frequencies, and initialize the
      //   forest of Huffman trees.
      cout << "Read frequencies\n";
      count = read_freqs(theTable, fp);
      //  forest->print();

      // Now, build the tree.
      cout << "Build the tree\n";
      theTree = buildHuff<char>(TreeArray, count);

      // Now, output the tree, which also creates the code table.
      cout << "Output the tree\n";
      buildcode(theTree->root(), theTable, prefix, 0, total);
      cout << "Average code length is "
           << total/(double)theTree->weight() << "\n";
      cout << "space used before Huff tree: " << total * 8<<endl;
      cout << "average space used after Huff tree: " << total/(double)theTree->weight() * 8<<endl;
      cout << "compression ratio = " << (total/(double)theTree->weight() * 8) / (total * 8)<<endl;




      // Finally, do the encode/decode commands to test the system.
      do_commands(theTree, theTable, fp);

      break;
    case 3:
    myfile.open("codeinstructions.txt");
    cin.get();
      a=0;
    while (a != -1) {
      cout << "Enter your Encode or Decode action followed by the value in quotes, or -1 to continue" << endl;
      getline(cin, cintext);
      if (cintext != "-1"){myfile << cintext << endl;}
      else{a = -1;}
      }
      myfile.close();
      fp = fopen("codeinstructions.txt","rt");
      do_commands(theTree, theTable, fp);

      break;
    case 4:
    cout << "Good bye!" << endl;
      done = true;
      break;

  }


}


  return 0;
}
