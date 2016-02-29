/*
Grammar Parser for FresnoSP16 Language (simple c++)
Lab 3
input: FresnoSP16 language program, p3data.txt
output: evaluation p3data.txt's
Myke Walker - Structures of Programming Languages
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

 //Initialization
int Exp(), Term(), Exp2(int), Term2(int), Fact();
void Declarations(), Declaration(string x), Statements(), Statement(string x), PrintSt(),Assign_st(string x);
string RWord();
string str;
int index=0;

// File Handling
ifstream fin ("p3data.txt", std::ifstream::in);

//Struct for type map
struct typemap{
  typemap()
  : id("0"), type("0"), value(0)
  {
      //Do nothing
  }
  string id;
  string type;
  int value;
}ids[25];







//Main Function
int main(){

  cout << "Entering Main Function\n";

  if (RWord() == "program"){
    cout << "--Program Found!, Interpreting File -- \n";
    Declarations();
    Statements();
    }
  else{
    cout << "---- No Program Found! ----\n";
    exit(1);
    }
  }

/*--------------
--Lab 2 Business
---------------*/


//Expression Function
int Exp(){
  char del;
  if (fin.peek() == 32){
    del = fin.get();
    del = fin.get();
    del = fin.get();
  }

  return Exp2(Term());
}


//Term Function
int Term(){
  return Term2(Fact());
}

// Expression 2 Function - takes char from Expression Function, Handles + & -
int Exp2(int inp){
  int result = inp;
  char a;
  fin.get(a);

//Prevents continuing past the EOF
  if (fin.eof() == false)
    {
       //checks if +
      if (a=='+')
        result = Exp2(result + Term());
      //checks if -
      else if (a=='-')
        result = Exp2(result - Term());
    }
  //returns
  return result;
}

// Term 2 Function - takes char from Term Function, Handles * & / & ^
int Term2(int inp){
  int result = inp;
  char a;
  fin.get(a);

  if (fin.eof() == false){
    //checks for multiplication
    if (a == '*')
      result = Term2(result * Fact());
    //checks for division
    else if (a == '/')
      result = Term2(result / Fact());
    //checks for mathematical operations not supported by function
    else if (a =='+' || a =='-' || a ==')')
      fin.putback(a);
    else if (a=='^'){
    //checks for Exponent operator, if found calculates using nested recursion
        result = pow(result,Term  ());
      }

  }
  return result;
}

//Factor Function - converts string or char to int for calculation functions
int Fact(){
  char a = fin.get() ;
  //If parenthesis found, uses nested recursion to solve the required
  //expression by following infix rules
  if (a=='('){
    return (Exp());
  }
  //converts string or char to int.
  return atoi(&a);
}

/*---------------------------------------------------------
Lab 3 Functions----------------------------------------------
-----------------------------------------------------------*/

//Declarations Function - Handles Detection of Variables for inits
void Declarations(){
  string temp;
  temp = RWord();
  if (temp == "begin"){//if no Declarations & Found Statement - exits function.
    return;
  }
  else if((temp == "int")||(temp == "double")){
    Declaration(temp);//Calls to handle the initilization
  }
  Declarations(); //Recursion


}

//Declaration function - Handles initilization of variables.
void Declaration(string x){

  char y = fin.get();
  if (y ==32){  //Removes empty space
    y=fin.get();
  }
    index = y-96; //converts ascii value to place in alphabet

  if (x== "int"){ //Initilizes Integer Variables
    ids[index].type = "int";
    ids[index].id = y;
    cout <<"New Variable Initalized! of Type: "<<ids[index].type<<" && named: "<<ids[index].id<<" at index: "<<index<<endl;

    if (fin.peek() == 44){ //if comma, remove comma and call again
      y=fin.get();
      Declaration(x);
    }

    if (fin.peek() == 59) {//if semicolon-removes from buffer and exits function
      y=fin.get();
      return;
    }

  }
  else if (x=="double"){//Initializes Double Variables
    ids[index].type = "double";
    ids[index].id = y;
    cout <<"New Variable Initalized! of Type: "<<ids[index].type<<" && named: "<<ids[index].id<<" at index: "<<index<<endl;


    if (fin.peek() == 44){//if comma, remove comma and call again
      y=fin.get();
      Declaration(x);
    }

    if (fin.peek() == 59){//if semicolon-removes from buffer and exits function
      y=fin.get();
      return;
    }

  }
}

//handles Execution of Statements
void Statements(){
  string temp;
  temp = RWord();
  if (temp == "end") {exit(1);} //if end, exits function
  Statement(temp);//Calls function to process Execution of statement
}

//Statement Function determines whether it is a print or assign stmt
void Statement(string x){
  if (x=="print"){//process print statement
    PrintSt();
  }
  else {Assign_st(x);}//processes Assign Statement
  Statements();//Recursion
}

//PrintSt Function, handles all print statements.
void PrintSt(){
  char temp = fin.get();
  temp = fin.get();
  if (isalpha(temp)){//Determines if buffer is alphabetical
    if (ids[temp-96].id != "0"){
      cout<<ids[temp-96].value<<endl; //calls printing of variable
    }//end if
    else{
      cout<<"Semantic Error, Variable not Initalized\n";
      exit(1);//if variable did not go through init, exits program.
    }//end else
  }//end if
  else{
    fin.putback(temp);//if it is not calling a variable, process the expression
    cout<<Exp()<<endl;
  }

}

//Assign_st Function handles Assigning statements to Variable
void Assign_st(string x){
  char tempc;
  tempc = x[0];//string to char
  index = tempc-96;//ascii value to alphabet index


  if (ids[index].id =="0"){
    cout << "Semantic Error, Variable not Initalized\n";
    exit(1);//if variable did not go through init, exits program.
  }
  else{

    ids[index].id = x; //name it
    ids[index].value = Exp(); //determine expression output
    ids[index].type = "int"; //give it type int
  }

}





//RWord Function Reads next word from file
string RWord(){
  string word;
  fin >> word;
  if (word == ";"){//if it encounters a semicolon,
    word = RWord();//Recursively call RWord to grab next word.
    }
    return word;
  }
