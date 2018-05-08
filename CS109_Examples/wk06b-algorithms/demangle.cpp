// $Id: demangle.cpp,v 1.10 2015-05-19 15:18:07-07 - - $

// Demangle a typeid(X).name() string

#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

using namespace std;

#include <cxxabi.h>
template <typename type>
string demangle (const type &object) {
   const char *const name = typeid (object).name();
   int status = 0;
   using deleter = void (*) (void*);
   unique_ptr<char,deleter> result {
      abi::__cxa_demangle (name, nullptr, nullptr, &status),
      std::free,
   };
   return status == 0 ? result.get() : name; 
}

class foo { }; 
class bar: foo { }; 
class baz: bar { }; 
template <typename T> class tmpl { T x; };

template <typename type>
void print_demangled (const string &str, const type &obj) {
   cout << str << " => " << sizeof obj << endl;
   cout << "   mangled:   " << typeid(obj).name() << endl;
   cout << "   demangled: " << demangle (obj) << endl;
}

#define DEMANGLE(X) print_demangled (#X, X())
int main() {
   using schar = signed char;
   using uchar = unsigned char;
   using ushort = unsigned short;
   using uint = unsigned int;
   using ulong = unsigned long;
   using map_string_int = map<string,int>;
   DEMANGLE (bool);
   DEMANGLE (char);
   DEMANGLE (uchar);
   DEMANGLE (schar);
   DEMANGLE (short);
   DEMANGLE (ushort);
   DEMANGLE (int);
   DEMANGLE (uint);
   DEMANGLE (long);
   DEMANGLE (ulong);
   DEMANGLE (float);
   DEMANGLE (double);
   DEMANGLE (size_t);
   DEMANGLE (foo);
   DEMANGLE (bar);
   DEMANGLE (baz);
   DEMANGLE (tmpl<int>);
   DEMANGLE (vector<string>);
   DEMANGLE (vector<int>);
   DEMANGLE (list<vector<long>>);
   DEMANGLE (map_string_int);
   return 0;
}

//TEST// demangle >demangle.out 2>&1
//TEST// mkpspdf demangle.ps demangle.cpp* demangle.out

