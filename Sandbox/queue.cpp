#include "queue.h"

 
void queue::push_back(const string& push_me){
   deq.push_back(push_me);
}
void queue::pop_front(){
   deq.pop_front();
}
string& queue::front() {
   return deq.front();
}
bool queue::empty() const{
   return deq.empty();
}

int main(){
   queue play;
   play.push_back("Got it to work");
   cout << play.front() << endl;
}
