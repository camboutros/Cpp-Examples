// $Id: tree-fnobj.cpp,v 1.1 2012-06-05 22:12:27-07 - - $

//
// Show a function object to perform an operation on each
// node in the tree.
//

#include <cstdlib>
#include <iostream>

using namespace std;

// equivalent to Java interface traverseobj
template <typename item_t>
struct traverseobj {
   virtual void operator () (item_t &) = 0;
};

template <typename item_t>
class tree {
   private:
      struct node {
         item_t item;
         node *left;
         node *right;
         node (item_t item): item (item), left (NULL), right (NULL) {}
      };
      node *root;
      void traverse_recur (node *curr, traverseobj<item_t> &);
      void delete_recur (node *curr);
   public:
      tree ();
      tree (const tree<item_t> &);
      tree<item_t> &operator= (const tree <item_t> &);
      ~tree ();
      void traverse (traverseobj<item_t> &);
};

template <typename item_t>
tree <item_t>:: tree () {
   // This ctor is junk, just to get a tree made and to
   // avoid an insert routine.  We really want to test the 
   // traverseobj.
   root = new node (6);
   root->left = new node (8);
   root->right = new node (10);
   root->left->left = new node (1);
}

template <typename item_t>
void tree<item_t>::traverse (traverseobj<item_t> &travobj) {
   traverse_recur (root, travobj);
}

template <typename item_t>
void tree<item_t>::traverse_recur (typename tree<item_t>::node *curr,
                                   traverseobj<item_t> &travobj) {
   if (curr == NULL) return;
   travobj (curr->item);
   traverse_recur (curr->left, travobj);
   traverse_recur (curr->right, travobj);
}

template <typename item_t>
tree<item_t>::~tree () {
   delete_recur (root);
}

template <typename item_t>
void tree<item_t>::delete_recur (typename tree<item_t>::node *curr) {
   if (curr == NULL) return;
   delete_recur (curr->left);
   delete_recur (curr->right);
}

class int_traverseobj: public traverseobj<int> {
   private:
      int value;
   public:
      int_traverseobj (): value (0) {}
      virtual void operator () (int &number) {value += number; }
      int get () { return value; }
};

int main () {
   tree <int> atree;
   int_traverseobj itrav;
   atree.traverse (itrav);
   cout << itrav.get () << endl;
   return EXIT_SUCCESS;
}

