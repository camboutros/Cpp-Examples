using namespace std;
#include<iostream>

int main() {
   int* p1 = NULL;
   int* p2 = nullptr;

   if (p1 == p2) 
   {
     cout << "NULL = nullptr in value " << endl; 
   }
   
   //cout << "Dereferencing 'NULL': " << *p1 << endl;  // RUNTIE_ERROR
   //cout << "Dereferencing 'nullptr': " << *p2 << endl; //RUNTIME_ERROR
}

