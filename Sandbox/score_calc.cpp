
#include <iostream>
using namespace std;
 
int main(){
   
   int right = 0;
   int wrong = 0;
   int blank = 0;
   double score;

   cout << " Hi! Please enter, in this order, # right, # wrong, and # blank: " << endl;;
   while ((right + blank + wrong) != 12){
      cin >> right;
      cin >> wrong;
      cin >> blank;
     
      if ((right + blank + wrong) != 12){
         cout << "Does not add up... try again: " << endl;
      }
   }
          
   score = (((right) - (wrong*.5))/12) * 100;
   cout << "Score equals " << score << "%" << endl;
   
}
