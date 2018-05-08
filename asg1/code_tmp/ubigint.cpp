// $Id: ubigint.cpp,v 1.11 2016-03-24 19:57:29-07 - - $

/////////////////////////////////////
// Name: Camellia Boutros          //
// CruzID: cboutros                //
// Class: CMPS 109                 //
// Title: asg1                     //
// Date: 4/4/2016                  //
/////////////////////////////////////

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "ubigint.h"
#include "debug.h"


ubigint::ubigint (unsigned long that): ubig_value (that) {
   DEBUGF ('~', this << " -> " << 5)
      p_v = {}; // fix
   unsigned char push_char;
   int work_num;
   while (that > 0 ) {
         work_num = that % 10;
         push_char  = 10 - work_num;
         that = that / 10;
         ubig_value.push_back(push_char);
   }
   u_vector_size = ubig_value.size(); // init u_vector_size
}

ubigint::ubigint (const string& that): ubig_value(0) {


  /////////// Initialize Container //////////////
  for(auto ritor = that.crbegin(); ritor != that.crend(); ++ritor){
       ubig_value.push_back(*ritor);
       
   //cout << "current digit = " << *ritor << endl;//test
      p_v.push_back(to_string((*ritor - '0' )));
  }
  /////////// Trim Trailing Zeros ///////////////
  while (ubig_value.size() > 0 and ubig_value.back() == 0 ){
     ubig_value.pop_back();  
  }
  /// Set    p_v ... ///////////////////
  //    p_v  //lazy way

  /// Set Vector Size ////////////////////
  u_vector_size = ubig_value.size();
}

ubigint ubigint::operator+ (const ubigint& that) const {
   auto u_itor = ubig_value.cbegin();
   auto u_titor = that.ubig_value.cbegin();
   int carry = 0;
   udigit_t sum_char;
   ubigint result;
   
 while(u_itor != ubig_value.end() and u_titor != that.ubig_value.end()){
        sum_char = (*u_itor + *u_titor - '0'); //conv down
              if(carry == 1) {
                   sum_char = sum_char + '1' - '0';
                   carry = 0; // reset carry
              }
              if ((sum_char) > 57) { // '57' is ASCII for '9'
                    sum_char = sum_char - 10 ;
                    sum_char = sum_char + 0;
                    carry = 1; // set carry
              }
        result.ubig_value.push_back((sum_char - '0'));
        ++u_itor;
        ++u_titor;   
 }
  
    ///////////// One arg is longer than the other... //////////
       while (u_itor != ubig_value.end()){
        sum_char = (*u_itor); 
        if(carry == 1) {
               sum_char = sum_char + '1' - '0';
               carry = 0; // reset carry
        }
        if ((sum_char) > 57) { // '57' is ASCII for '9'
               sum_char = sum_char - 10 ;
               sum_char = sum_char + 0;
               carry = 1; // set carry
        }
        result.ubig_value.push_back((sum_char - '0'));
        ++u_itor;
   } 
   while (u_titor != that.ubig_value.end()){
        sum_char = (*u_titor); 
        if(carry == 1) {
           sum_char = sum_char + '1' - '0';
           carry = 0; // reset carry
        }
        if ((sum_char) > 57) { // '57' is ASCII for '9'
             sum_char = sum_char - 10 ;
             sum_char = sum_char + 0;
             carry = 1; // set carry
        }
        result.ubig_value.push_back((sum_char - '0'));
        ++u_titor;
   }
   //////////////////////////////////////////////////////////////
   //////////////// Final Carry /////////////////////////////////
   if (carry == 1 ) {
         result.ubig_value.push_back('1' - '0'); // leftover
   } 
  ////////////////////////////////////////////////////////
   ///// Make a printable version of ubigint result.... /////////
   udigit_t holdchar;   
   auto ritor = result.ubig_value.cbegin();
   while (ritor != result.ubig_value.cend()){
        holdchar = *ritor;
        result.   p_v.push_back( to_string(holdchar));
        ritor++;
   }
   //////////////////////////////////////////////////////////////
   return result; 
}


ubigint ubigint::operator- (const ubigint& that) const {
   auto u_itor = ubig_value.cbegin();
   auto u_titor = that.ubig_value.cbegin();
   int borrow = 0;
   udigit_t diff_char;
   ubigint r; //result
   int workspace = 0; // to do arithmetic 
 
   
 while(u_itor != ubig_value.end() and u_titor != that.ubig_value.end()){
         diff_char = *u_itor;
        
         if(borrow == -1) {
               diff_char = diff_char - '1' + '0';
               borrow = 0; // reset carry   
            
         }
         
        if (diff_char < *u_titor){ 
              diff_char = diff_char + 10;
              diff_char = diff_char + 0;
           
              workspace = (diff_char - '0');
           
              workspace = workspace - (*u_titor - '0') ;
             
              borrow = -1; // set borrow-carry
              diff_char = (workspace + '0');
         } else {  
               
                workspace = (diff_char);
                workspace = workspace - (*u_titor - '0'); 
                diff_char = ((workspace));     
         }

         r.ubig_value.push_back((diff_char - '0'));
         ++u_itor;
         ++u_titor;
         
 }

 ///////////// One arg is longer than the other... //////////
    while(u_itor != ubig_value.end()){
        diff_char = (*u_itor);

        if(borrow == -1) {
             if (diff_char != '0') { // or equivalent... mess
                 diff_char = diff_char - '1' + '0';
                 borrow = 0; // reset carry
              } // else, do not reset carry, push zero. 
        }

        r.ubig_value.push_back((diff_char - '0'));
        ++u_itor;
   } 
   while(u_titor != that.ubig_value.end()){
        diff_char = (*u_titor);
        if(borrow == -1) {
             diff_char = diff_char - '1' + '0';
             borrow = 0; // reset carry
        }
        
        r.ubig_value.push_back((diff_char - '0'));
        ++u_titor;
   }  
     // if (borrow == -1 ) {
     //      cout << "WARNING: CARRY OVERFLOW" << endl; //fix later
   //}
 ////////////////////////////////////////////
 ///////// Trim Trailing Zeroes /////////////
   while (r.ubig_value.size() > 0 and r.ubig_value.back() == 0 ){
     r.ubig_value.pop_back();  
  }
 //////////////////////////////////////////////////////////////
 ///// Make a printable version of ubigint result....   ///////
   udigit_t holdchar;
   auto ritor = r.ubig_value.cbegin();
   while (ritor != r.ubig_value.cend()){
        holdchar = *ritor;
        r.p_v.push_back(to_string(holdchar));
        ritor++;
   }
   //////////////////////////////////////////////////////////////
   return r; // test
}

ubigint ubigint::operator* (const ubigint& that) const {
   ubigint product;
   int expected_size = (u_vector_size + that.u_vector_size);
   for (int i = 0; i < expected_size; ++i){
      product.ubig_value.push_back('0');
   }
   udigit_t temp;
   int carry = 0;
   int m = 0; //counter for i iterator
   int n = 0;  //counter for j iterator
   
   
 for (auto i = ubig_value.cbegin(); i != ubig_value.cend(); i++){
  carry = 0;
  for(auto j=that.ubig_value.cbegin();j!=that.ubig_value.cend(); j++){
     temp = (product.ubig_value.at(m+n)+((*i-'0')*(*j-'0'))+carry);
     
     
     product.ubig_value.at(m+n) = (((temp - '0')% 10) + '0');
     
     carry = (((temp - '0')/ 10) + '0');
     n = n + 1;  
     
   } //end inner for-loop
  product.ubig_value.push_back(carry);
  
  m = m + 1;
 } //end outer for-loop
 return product; // test
}
void ubigint::multiply_by_2() {
  *this = *this + *this;
}

void ubigint::divide_by_2() {
   *this = *this / *this;
}

int ubigint::get_vector_size() {
      return u_vector_size;
}

struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, ubigint divisor) {
   // Note: divisor is modified so pass by value (copy).
   ubigint zero {'0'};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {'1'};
   ubigint quotient {'0'};
   ubigint remainder {dividend}; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
 auto u_itor = ubig_value.cbegin();
 auto u_titor = that.ubig_value.cbegin();
 while(u_itor != ubig_value.end() and u_titor != that.ubig_value.end()){
       if (*u_itor != *u_titor){
             return false;
       } // else...
       ++u_itor;
       ++u_titor;  
 }
 return true;
}

bool ubigint::operator< (const ubigint& that) const {
 auto u_i = ubig_value.crbegin();
 auto u_t = that.ubig_value.crbegin();
 if (this->u_vector_size != that.u_vector_size){
  return ((this->u_vector_size < that.u_vector_size)? true:false);
 } else { //must compare magnitude;
    while(u_i != ubig_value.crend() and u_t != that.ubig_value.crend()){
       if (*u_i > *u_t){
            return false;
        } // else... 
        ++u_i;
        ++u_t;  
    }
 }
 return true; 
}
ostream& operator<< (ostream& out, const ubigint& that) { 
  string printable;
  for (auto i = that.   p_v.rbegin(); i != that.   p_v.rend(); i++){
          printable = printable.append( *i);
    }
   return out << printable; 
}

