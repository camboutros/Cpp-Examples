// $Id: show-get-libc.cpp,v 1.1 2015-08-11 16:52:00-07 - - $

#include <iostream>
using namespace std;

#include <gnu/libc-version.h>

int main() {
   cout << "libc_version = " << gnu_get_libc_version() << endl;
}
