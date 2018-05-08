// $Id: listmap.tcc,v 1.7 2015-04-28 19:22:02-07 - - $

/////////////////////////////////////
// Name: Camellia Boutros          //
// CruzID: cboutros                //
// Class: CMPS 109                 //
// Title: asg3                     //
// Date: 4/29/2016                 //
/////////////////////////////////////

#include "listmap.h"
#include "trace.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
               
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);
   TRACE('t', get_anchor_next() << " = anchor next" << endl);
   TRACE('t', get_anchor_pointer() << " = anchor itself" << endl); 
   node* anchor = get_anchor_pointer();
  
   if (begin() == end()){ //list empty
     TRACE('t', "List empty" << endl);
     node* curr = new node(anchor, anchor, pair);
     set_anchor_next(curr);
     set_anchor_prev(curr);
     TRACE('t', "First insertion: pntr to insrt = " << curr << endl);
     TRACE('t', "\"Begin\" returns " << (*begin()) << endl); 
   } else {
      
      iterator found = find(pair.first);
      if (found != end() ) { //key already in list; alter

         TRACE('t', "FOUND: value pair = "<< (*found) << endl);
        
         (*found).second = pair.second;
         TRACE('t', "Found's newSECOND: "<<found->second << endl);
      } else { //key not in list
         TRACE('t', "Key not in list " << endl); // insert
         // Search for proper position //
        
         string key_in_list = begin()->first; 
         iterator curr = begin();
         TRACE('t', "First key = " << begin()->first << endl); //test
         string key_to_insert = pair.first; 
         while ((less(key_in_list, key_to_insert)) and (curr != end())){
            key_in_list = (*curr).first;
            ++curr;
         } 
           
         node* current = curr.get_assoc_node();   
         node* prev = ((curr.get_assoc_node())->prev);
         // Perform Insertion //
         node* to_insert = new node(current, prev, pair);
        
         prev->next = to_insert;
         curr.get_assoc_node()->prev = to_insert;
      }
   }
   return iterator();
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) const {
   TRACE ('l', that);
   iterator curr = const_cast<listmap<Key,Value,Less>*>(this)->begin(); 
   
  while (curr != const_cast<listmap<Key,Value,Less>*>(this)->end() and (*curr).first != that){
     ++curr; 
  }
   
   return curr;
   //return iterator();
}


template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find_by_value (const mapped_type& that) const {
   TRACE ('l', that);
   iterator curr = const_cast<listmap<Key,Value,Less>*>(this)->begin(); 
   
  while (curr != const_cast<listmap<Key,Value,Less>*>(this)->end() and (*curr).second != that){ 
     ++curr; 
  }
   
   return curr;
   //return iterator();
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('l', &*position);
   node* current = position.get_assoc_node();
   node* nxt = (position.get_assoc_node())->next;
   node* prv = (position.get_assoc_node())->prev;
   prv->next = nxt;
   nxt->prev = prv;
   
   delete current;
    
   return iterator();
}
 
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::display_all () { 
   if (begin() == end()){ // list empty
      cout << "List empty" << endl; //test 
   } else {
      iterator curr = begin();
      while (curr != end()){
         cout << (*curr).first << " = " << (*curr).second << endl;      
         ++curr;
      }
   }
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;

}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

