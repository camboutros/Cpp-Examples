// $Id: addresses.cpp,v 1.1 2012-06-05 22:12:27-07 - - $

#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

#define PRINT(SYMBOL,DESCR) { \
        cout << setbase(16) << setw(8) << SYMBOL \
             << ": " << #SYMBOL << " - " << DESCR << endl; }

//extern void *_START_;
extern void *_start;
extern void *_etext;
extern void *_edata;
extern void *_end;
//extern void *_END_;
extern char **environ;
static int init_var = 6;
static int uninit_var;

string fmt (const string &text, int value) {
   ostringstream descr;
   descr << text << value;
   return descr.str();
}

void stack (int level) {
   if (level < 5) stack (level + 1);
   PRINT (&level, fmt ("address of a stack variable at level ", level));
}

void *stack_bottom (char **start) {
   for (; *start != NULL; ++start) {}
   --start;
   char *startstr = *start;
   while (*startstr != '\0') ++startstr;
   return startstr;
}

int main (int argc, char **argv) {
   int main_local;
   //PRINT (NULL, "null pointer");
   cout << endl << "Addresses of some local variables:" << endl;
   stack (1);
   PRINT (&main_local, "address of a local variable in main");
   PRINT (&argc, "address of argc");
   PRINT (&argv, "address of argv");
   PRINT (argv, "address of arg vector");
   PRINT (environ, "address of environ vector");
   PRINT (stack_bottom (environ), "byte at bottom of stack");
   cout << endl << "Addresses of some static variables:" << endl;
   //PRINT (&_START_, "start of text segment");
   PRINT (&_start, "start of program text");
   PRINT (&_etext, "end of program text");
   PRINT (&init_var, "address of an init static variable");
   PRINT (&_edata, "end of init data segment");
   PRINT (&uninit_var, "address of an uninit static variable");
   PRINT (&_end, "end of uninit data segment");
   //PRINT (&_END_, "end of program segment");
   cout << endl << "Addresses of some heap variables:" << endl;
   for (int count = 0; count < 10; ++count) {
      int *heap_variable = new int[1000];
      PRINT (heap_variable, fmt ("heap variable ", count));
   }
   return 0;
}

