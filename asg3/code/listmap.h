// $Id: listmap.h,v 1.11 2015-04-28 19:22:02-07 - - $

/////////////////////////////////////
// Name: Camellia Boutros          //
// CruzID: cboutros                //
// Class: CMPS 109                 //
// Title: asg3                     //
// Date: 4/29/2016                 //
/////////////////////////////////////

#ifndef __LISTMAP_H__
#define __LISTMAP_H__

#include "xless.h"
#include "xpair.h"

template <typename Key, typename Value, class Less=xless<Key>>
class listmap {
   public:
      using key_type = Key;
      using mapped_type = Value;
      using value_type = xpair<const key_type, mapped_type>;
   private:
      Less less;
      struct node;
      struct link {
         node* next{};
         node* prev{};
         link (node* next, node* prev): next(next), prev(prev){
            //cout << " head link init " << endl; //test
         }
      };
      struct node: link {
         value_type value{};
         node (node* next, node* prev, const value_type&);
      };
      node* anchor() { //cout << "anchor init " << endl; //test
                       return static_cast<node*> (&anchor_); }
      link anchor_ {anchor(), anchor()};
   public:
      class iterator;
      listmap(){};
      listmap (const listmap&);
      listmap& operator= (const listmap&);
      ~listmap();
      iterator insert (const value_type&);
      iterator find (const key_type&) const;
      iterator find_by_value (const mapped_type&) const;
      iterator erase (iterator position);
      iterator begin() { return anchor()->next; }
      iterator end() { return anchor(); }
      bool empty() const { return begin() == end(); }
      node* get_anchor_pointer() {return anchor();} 
      node* get_anchor_next(){return anchor()->next;}
      void display_all();
      
      //node* top() const{return anchor()->next;}
      //node* get_anchor_prev()
      void set_anchor_next(node* me_next) {anchor()->next = me_next;}
      void set_anchor_prev(node* me_last) {anchor()-> prev = me_last;}
};


template <typename Key, typename Value, class Less=xless<Key>>
class listmap<Key,Value,Less>::iterator {
   private:
      friend class listmap<Key,Value>;
      listmap<Key,Value,Less>::node* where {nullptr};
      iterator (node* where): where(where){};
   public:
      iterator(){}
      value_type& operator*();
      value_type* operator->();
      iterator& operator++(); //++itor
      iterator& operator--(); //--itor
      void erase();
      bool operator== (const iterator&) const;
      bool operator!= (const iterator&) const;
      node* get_assoc_node(){return where;}
      
};

#include "listmap.tcc"
#endif

