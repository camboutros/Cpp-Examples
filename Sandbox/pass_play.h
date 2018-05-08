//Header guard. Title can be any unique name. By convention, use the name of the header file.

#ifndef __PASS_PLAY_H
#define __PASS_PLAY_H

//Begin content of .h file
#include <iostream>
#include <vector>
using namespace std;

template<typename T>
using vec_t = vector<T>;

string& update(string&);
void read(const string&);

   
//End of header guard:
#endif
