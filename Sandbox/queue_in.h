#ifndef __QUEUE__H_
#define __QUEUE__H_

#include <iostream>
#include <deque>

using namespace std;

class queue{
   private:
      deque<string> deq;
   public:
      void push_back(const string& item);
      void pop_front();
      string& front();
      bool empty() const;
};
 
inline void queue::push_back(const string& item){
   deq.push_back(item)
};
inline void queue::pop_front(){
   deq.pop_front()
};
inline string& queue::front() {
   return deq.front()
};
inline bool queue::empty(){
   return deq.empty()
};
#endif
