#include <iostream>
#include <vector>
using namespace std;

//This is to help me understand the difference between passing by value, pasisng by reference, and passing by const value


template<typename item>
void swap(item & first, item & second){
   item temporary = first;
   first = second;
   second = temporary;
};

int main(){

   const int age(22); // immortality
   cout << "My constant age is " << age << endl;
   //age = 12; // error
   
   
   
   vector<string> vec;
   string okay = "okay";
   string not_okay = "not_okay";
   vec.push_back(okay);
   vec.push_back(not_okay);
   for(auto i : vec){
      cout << "Elem: " << i << endl;
   }
   swap(okay, not_okay);
   for (auto i : vec){
      cout << "Elem: " << i << endl;
   }

   
}

