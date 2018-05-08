// $Id: bigint.cpp,v 1.74 2016-03-24 19:30:57-07 - - $

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

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)

}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0)); 
      v_s = uvalue.get_vector_size();
}

bigint bigint::operator+ () const {
   return *this;
}

bigint bigint::operator- () const {
   return {uvalue, not is_negative};
}


bigint bigint::operator+ (const bigint& that) const {
   ubigint ubig_result;
   bigint final_result;

   if ((is_negative == that.is_negative)) { 
        ubig_result = uvalue + that.uvalue;
        final_result.is_negative = is_negative;
   }
   else { 
     if (uvalue > that.uvalue ){
        final_result.is_negative = is_negative; 
        ubig_result = uvalue - that.uvalue;
     } else if (uvalue < that.uvalue ) {
        final_result.is_negative = that.is_negative;
        ubig_result = that.uvalue - uvalue;
     }
   }

   final_result.uvalue = ubig_result;
   return final_result;
}

bigint bigint::operator- (const bigint& that) const {
 ubigint result;
 bigint f; // final result

 if (is_negative == that.is_negative) { //signs are the same
     if (uvalue < that.uvalue){ 
     f.is_negative = not is_negative; 
     result = that.uvalue - uvalue;
      
    } else if (uvalue > that.uvalue ) {
       
      f.is_negative = that.is_negative;
      result = uvalue - that.uvalue;
        
    }
 } else { // signs are different
  result = uvalue + that.uvalue;
  f.is_negative =((uvalue > that.uvalue)? is_negative:that.is_negative);
 }
  f.uvalue = result;
  return f;
}

bigint bigint::operator* (const bigint& that) const {
   bigint result;
   if (not(is_negative == that.is_negative)){
       result.is_negative = true; // - * + = -
   } else if (is_negative == true){ ///signs are the same and both -
       result.is_negative = false; // resulting sign is positive
   } else { result.is_negative = false; } // otherwise is +;
   result. uvalue = uvalue * that.uvalue;
   return result;
}

bigint bigint::operator/ (const bigint& that) const {
   bigint result = uvalue / that.uvalue;
   return result;
}

bigint bigint::operator% (const bigint& that) const {
   bigint result = uvalue % that.uvalue;
   return result;
}

bool bigint::operator== (const bigint& that) const{
   
   if (not((is_negative == that.is_negative) and (v_s == that.v_s))){
         return false;
   }
   if (not(uvalue == that.uvalue)) { //difference found 
         return false; //bigints not the same
   }

   return true; 

}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative){
      if (is_negative == true){ // - < +
        return true;
      } else { return false; } // + != -
   }
   // v_s = vector_size
   else if ((is_negative == that.is_negative) and (v_s == that.v_s)){
        return (uvalue < that.uvalue);
   } else if (   v_s < that.v_s) {
        return true;
   } else { return false;}
   
}

ostream& operator<< (ostream& out, const bigint& that) {
   return out << "bigint(" << (that.is_negative ? "'-'" : "'+'")
              << that.uvalue << ")";
}

