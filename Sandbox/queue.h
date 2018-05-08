#ifndef __QUEUE__H_
#define __QUEUE__H_

#include <iostream>
#include <deque>

using namespace std;

class queue{
   private:
      deque<string> deq;
   public:
      void push_back(const string&);
      void pop_front();
      string& front();
      bool empty() const;
};
#endif
