// $Id: hello.cpp,v 1.10 2016-03-24 17:33:55-07 - - $

//
// NAME
//    hello - the classic ``hello world'' program
//
// SYNOPSIS
//    hello
//
// DESCRIPTION
//    Prints out the message ``Hello, world!''.
//

#include <cstdlib>
#include <iostream>

using namespace std;

int main (int argc, char **argv) {
   (void) argc; // warning: unused parameter 'argc'
   (void) argv; // warning: unused parameter 'argv'
   cout << "Hello, world!" << endl;
   return EXIT_SUCCESS;
}

//TEST// ./hello 1>hello.out 2>hello.err
//TEST// echo EXIT STATUS = $? >hello.exit
//TEST// catnv hello.cpp.log hello.out hello.err hello.exit >hello.lis
//TEST// rm -v hello.cpp.log hello.out hello.err hello.exit
//TEST// mkpspdf hello.ps hello.cpp hello.lis

