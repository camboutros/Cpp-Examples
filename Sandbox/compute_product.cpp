#include <iostream>
#include <vector>

using namespace std;

vector<double> compute_product(vector<double> vec1, vector<double> vec2){
   if (vec1.size() != vec2.size()){
      cout << "Error" << endl; 
   }
   vector<double> product;
   for (int i = 0; i != vec1.size(); ++i){
      product.push_back((vec1.at(i) * vec2.at(i)));
   }
   return product;
}

int main(){
   vector<double> vec1;
   vector<double> vec2;
  
   vec1.push_back(45); 
   vec1.push_back(23); 
   vec1.push_back(56); 
   
   vec2.push_back(9); 
   vec2.push_back(10); 
   vec2.push_back(26);
 
   vector<double> product;
   product = compute_product(vec1, vec2);
   
   for (int i = 0; i !=  product.size(); ++i){
      cout << vec1.at(i) << " x " << vec2.at(i) << " = " << product.at(i) << endl;
   }
}
