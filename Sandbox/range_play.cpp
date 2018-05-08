#include<iostream>
#include<map>
#include<vector>
using namespace std;

main() {
   map<string, vector<int>> map;
   vector<int> vec_int;
   vec_int.push_back(1);
   vec_int.push_back(2);
   vec_int.push_back(3);
   map["one"] = vec_int;
   //friend ostream& operator<< (ostream& out, string string_to_out);
   for (const auto& key_value_pair : map) //QUESTION: doesn't make a diff if I use auto or auto&?
   {
      //cout << "First iterator value: " << *key_value_pair << endl; //error: you tryna spit out a pair?
      cout << "First key: '" << key_value_pair.first << "' contains: " << endl; 
      
      for(auto value : key_value_pair.second) //auto takes type int
      {  
          
         //cout << "'value' derefed: " << *value << endl; //Error: cannot deref an int
         cout << "\tValue: " << value << endl;
      }
   }
 
   int arr[] = {1,2,3,4,5,};
   for(int& element : arr)
   {
      element = element*element;
      cout << "Element: " << element << endl;
   } 

}//close main


