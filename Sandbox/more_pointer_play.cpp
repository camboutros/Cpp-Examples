#include<iostream>
using namespace std;

int main(){
   int item = 42;
   int* ptr = &item;

   
   
   cout << "Item = " << item << endl;
   cout << "Pointer to Item = " << ptr << endl;
  
   cout << "*ptr++ = " << *ptr++ << "Bad; displays expected value but then increments pointer. "<< endl;
     
   item = 42;
   ptr = &item;
   cout << "*++ptr = " << *++ptr << endl;
   
   item = 42;
   ptr = &item;
   cout << "++*ptr = " << ++*ptr << endl; 
   
   //cout << "ptr++* = " << ptr++* << endl; //Error
}

