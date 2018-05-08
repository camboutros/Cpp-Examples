#include "pass_play.h"
using namespace std;


int main(){
   vec_t<string> vec;
   string okay = "okay";
   vec.push_back(okay);
   cout << "Item: " << vec.at(0) << endl;
   string pass_me = "Hot potato!";
   read(pass_me);
   update(pass_me);
   read(pass_me);

   
  
 

   int a = 4;
   int b = 0;
   //try {
   //   cout << (a/b);
   //} catch(exception& e){
   //   cout << "error" << endl;
   //}
   double i = 4.0;
   double j = 0.0;
   cout << "4.0/00.0 = " << (i/j) << endl;;
}
string& update(string& mess_with_me){
   mess_with_me += " #IMPROVED";
   return mess_with_me;
}

void read(const string& do_not_mess_with_me){
   //try{
   //   do_not_mess_with_me += " #did not work";
   //} 
   //catch(runtime_error& e){
   //   cerr << "Cannot modify a const ref arg" << endl;
   //}
   cout << "Reading: " << do_not_mess_with_me << endl;
}

