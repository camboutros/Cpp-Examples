#include <iostream>
using namespace std;
#include <list>

int main(){
 list<int> lis;
 lis.push_back(1);
 lis.push_back(2);
 lis.push_back(3);
 
 for(auto i : lis){
    cout << "i = " << i << endl;
 }
}
