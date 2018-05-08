// $Id: file_sys.cpp,v 1.5 2016-01-14 16:16:52-08 - - $

/////////////////////////////////////
// Name: Camellia Boutros          //
// CruzID: cboutros                //
// Class: CMPS 109                 //
// Title: asg2                     //
// Date: 4/13/2016                 //
/////////////////////////////////////

#include <iostream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

#include "debug.h"
#include "file_sys.h"
#include "util.h"

int inode::next_inode_nr {1};

struct file_type_hash {
   size_t operator() (file_type type) const {
      return static_cast<size_t> (type);
   }
};

ostream& operator<< (ostream& out, file_type type) {
   static unordered_map<file_type,string,file_type_hash> hash {
      {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
      {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
   };
   return out << hash[type];
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");  

   /////////// Initialize Root ////////////////
   inode_ptr ptr = make_shared<inode>(file_type::DIRECTORY_TYPE);
   root = ptr;
   DEBUGF('i', "ptr value: " << ptr << endl); 
    
   /////////// Initialize CWD /////////////////
   cwd = root;
   
   root -> make_root(root);
 
    DEBUGF ('i', "After init: root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");  
}

const string& inode_state::prompt() { return prompt_; }

inode_ptr inode_state::get_root() {return root;}

inode_ptr inode_state::get_current() {return cwd;}

void inode_state::set_current(inode_ptr new_cwd_ptr){
   cwd = new_cwd_ptr;
}
ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

void inode_state::set_prompt(const string& new_prompt) { 
     prompt_ =  new_prompt;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           f_type = "plain";
           DEBUGF('i',  "File initialized" << endl);
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           f_type = "directory";
           DEBUGF('i', "Director initialized" << endl);
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

int inode::get_inode_size() {
   return contents->size();
}

void inode::make_root(inode_ptr root) {
   contents -> make_root(root); // pass function to directory level
}
bool inode::is_empty(){
   return contents->is_empty();
}

void inode::display_contents(int i, size_t t, string& n) {
   contents -> display_contents(i, t, n);
}
void inode::display_all(int i_num, size_t size, string& name  ){
   contents->display_all(i_num, size, name);
}

void inode::mkdir(const string& dirname){
   try{
      contents-> mkdir(dirname);
   }
   catch (file_error e) {
      cout << "yshell: Directory already exists" << endl;
   }
}

void inode::init_dir(inode_ptr parent, inode_ptr current){
   contents->init_dir(parent, current);
}
 
void inode::mkfile(const string& filename){
   contents->mkfile(filename);
}  


inode_ptr inode::find_inode(const string& dirname){
   return contents->find_inode(dirname);
} 

wordvec inode::return_list_of_contents(){
   return contents->return_list_of_contents();
}   

string inode::dad_what_my_name(inode_ptr child){
   return contents->dad_what_my_name(child);
}

string inode::pwd(){
   return contents->pwd();
}
 
void inode::writefile(wordvec words){
   contents->writefile(words);
}
 
void inode::readfile(){
   wordvec data = contents->readfile();
   for (auto i = data.begin(); i != data.end(); ++i){
      cout << *i << " ";
   } 
   cout << endl;
}

void inode::remove(const string& filename){
   contents->remove(filename);
}
 
void inode::remove_rec(){
   contents->remove_rec();
}

const wordvec inode::get_names(){
   return contents->get_names();
}

inode_ptr inode::get_parent(){
   return contents->get_parent();
}

file_error::file_error (const string& what):
          
            runtime_error (what) {
}

size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   if (not is_empty()){
      for (auto word : data){ //returns # of char
         for (auto letter : word){
            ++size;
         }
      }
   }      
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   data = words;
}

void plain_file::remove (const string&) {
   throw file_error ("is a plain file");
}

void plain_file::remove_rec(){
   inode_ptr parent = get_parent();  
   parent->remove_rec();
   //throw file_error("is a plain file");
}

const wordvec plain_file::get_names(){
   throw file_error("is a plain file");
}

inode_ptr plain_file::mkdir (const string&) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::mkfile (const string&) {
   throw file_error ("is a plain file");
}

bool plain_file::is_empty() const{
   return data.empty(); // returns true if file is empty
}

void plain_file::make_root(inode_ptr root) {
   throw file_error ("is a plain file");
}

void plain_file::display_contents(int i, size_t s, string& n  ){
   DEBUGF('i', "Inode Number:   Size:    Filename:   " << endl);
   
   cout << "\t" << i << "\t"; //inode_nr
   cout << s << "   "; //size
   cout << n << endl; //name
}

void plain_file::display_all(int i_num, size_t size, string& name  ){
   display_contents(i_num, size, name);
}

wordvec plain_file::return_list_of_contents(){
   throw file_error("is a plain file");
}

inode_ptr plain_file::get_myself(){
   throw file_error ("is a plain file");
}

inode_ptr plain_file::get_parent(){
   throw file_error ("is a plain file");
}

void plain_file::init_dir(inode_ptr parent, inode_ptr me){
   throw file_error ("is a plain file");
}


inode_ptr plain_file::find_inode(const string& dirname){
   throw file_error("is a plain file");
}

string plain_file::dad_what_my_name(inode_ptr child){
   throw file_error("is a plain file");
}

string plain_file::pwd(){
   throw file_error("is a plain file");
}


size_t directory::size() const {
   size_t size = dirents.size();
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& directory::readfile() const {
   throw file_error ("is a directory");
}

void directory::writefile (const wordvec&) {
   throw file_error ("is a directory");
}

void directory::remove (const string& name) {
   DEBUGF ('i', "Attempting to rm " << name << endl);
   inode_ptr target = find_inode(name);
   if (target->f_type == "plain"){
      dirents.erase(name);
      

   } else if (target->f_type == "directory"){
      if (not (target->is_empty())){
         cout << "Directory must be empty to remove" << endl;
      } else {
         dirents.erase(name);
        
      }
   }
}

void directory::remove_rec(){
   
   if (not is_empty()){
      for (auto i = dirents.begin(); i != dirents.end(); ++i){ 
      
         if ((i->first != ".") and (i->first != "..")){
            inode_ptr target = i->second;
            string name = i->first;
            
            if (target->f_type == "plain"){
                
                dirents.erase(name);
                
            } else if (target->f_type == "directory"){
                  
               if (not (target->is_empty())){
                  
                  target->remove_rec();
                  
               } else {
                  
                  remove(name); 
                  
                 // dirents.erase(name);
               }
            }
          
         } // close while loop
      } //close for loop
       
            
         
         
         // wordvec hit_list; //list of contents by name
         
         // try {
         //    hit_list = target->get_names();
         //   }
         //catch (const bad_alloc&){
         //     cout << "Bad alloc on vec" << endl;//test
         // }
         //        for (auto i : hit_list){ //test
         //       cout << i << "   test " << endl; //test
         //   } ///test
         //for (auto i = hit_list.cbegin(); i != hit_list.cend(); ++i){
         //      target->remove_rec();
         // }
        
         ///////////
   } else {
      try {
         inode_ptr parent = get_parent(); 
         inode_ptr current = get_myself();
         string name = parent->dad_what_my_name(current);
         parent->remove(name);
      }
      catch (const bad_alloc&){
           cout << "Bad Alloc" << endl;
      }
   } //close else   
}

const wordvec directory::get_names(){
   cout << "Hit " << endl; //test
   wordvec targets;
   for(auto i = dirents.begin(); i != dirents.end(); ++i){
      if((i->first != ".") and (i->first != "..")){
         targets.push_back(i->first);
         cout << "pushing back" << i->first << endl;
      } 
   }
   return targets;
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   for (auto itor = dirents.begin(); itor != dirents.end(); ++itor){
      if(itor->first == dirname){
         if (itor->second->f_type == "directory"){
            throw file_error("Directory or file already exists.");
            
         } else if (itor->second->f_type == "plain"){
            throw file_error("Exists"); // fix
            
         } 
      }
   }
   
   inode_ptr parent = get_myself(); 
   
 inode_ptr new_inode_ptr=make_shared<inode>(file_type::DIRECTORY_TYPE);
   dirents.insert(pair<string, inode_ptr>(dirname, new_inode_ptr));
   
   new_inode_ptr -> init_dir(parent, new_inode_ptr); 

   
   return nullptr;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr new_inode_ptr = make_shared<inode>(file_type::PLAIN_TYPE);
   dirents.insert(pair<string, inode_ptr>(filename, new_inode_ptr));   
   return nullptr;
}

bool directory::is_empty() const{
   return (dirents.size() <= 2) ;//not sure
}

void directory::make_root(inode_ptr root) {
   
   inode_ptr root_ptr = root;
   root_ptr = root;

   dirents.insert(pair<string, inode_ptr>(".", root_ptr));
   dirents.insert(pair<string, inode_ptr>("..", root_ptr));
   
}

void directory::display_contents(int i, size_t t, string& n){
    DEBUGF('i', "Number: \t Size: \t Name: \t" << endl);

    for (auto i = dirents.begin(); i != dirents.end(); ++i){
       inode_ptr i_ptr = i->second;
       cout << "\t" << i_ptr->get_inode_nr() <<"\t";
       cout << i_ptr->get_inode_size() <<"   ";
       if (i_ptr->f_type == "directory"){
          if ((i->first == "." ) or (i->first == "..")){
             cout << i->first << endl;
          } else if ((i->first != ".") and (i->first != "..")){
             cout << i->first << "/" <<  endl;
          }
       } else if (i_ptr->f_type == "plain"){
          cout << i->first << endl;
       }
   }
} 
void directory::display_all(int i_num, size_t size, string& name  ){ 
   display_contents(i_num, size, name);
   for (auto i = dirents.begin(); i != dirents.end(); ++i){
      if ((i->first != ".") and (i->first != "..")){
         cout << "\n" << name << ":" << endl;
         i->second->display_contents(i_num, size, name);
         i->second->display_all(i_num, size, name);
      }
   }
}

inode_ptr directory::get_myself(){
   inode_ptr return_ptr;
   if(dirents.find(".")== dirents.end()){
      throw file_error("Error: directory not found");
   } else {
      return_ptr = dirents.find(".")->second;
   }
   return return_ptr;
}

inode_ptr directory::get_parent(){
   inode_ptr return_ptr;
   if( dirents.find("..") == dirents.end()){
      throw file_error("Error: directory not found");
   } else {
      return_ptr = dirents.find("..")->second;
   }
   return return_ptr;
}

void directory::init_dir(inode_ptr parent, inode_ptr me){
   dirents.insert(pair<string, inode_ptr>("..", parent));
   dirents.insert(pair<string, inode_ptr>(".", me));  
   
 
}

inode_ptr directory::find_inode(const string& dirname){
     
   inode_ptr return_ptr;
   if(dirents.find(dirname) == dirents.end()){
      
      throw file_error("Error: directory not found");
   } else {
      return_ptr = dirents.find(dirname)->second;
   }
   
   return return_ptr;
}

wordvec directory::return_list_of_contents(){
   wordvec list_result;
   for(auto itor = dirents.begin(); itor != dirents.end(); itor++){
      list_result.push_back(itor->first);
   }
   return list_result;
}

string directory::dad_what_my_name(inode_ptr child){
   for (auto itor = dirents.begin(); itor != dirents.end(); ++itor){
      
      if(itor->second == child){
         if(itor->first == "."){
            return "."; //make sure this doesn't break!
         } else {
            return itor->first;
         }
      }
   } 
   throw file_error("Something went wrong...");
   return "Something went wrong.";
}

string directory::pwd(){
   wordvec pathname;
   string path;
   inode_ptr current = get_myself();
   inode_ptr parent = get_parent();
   while (current != parent){
      string name = parent->dad_what_my_name(current);
      pathname.push_back(name);
      current = parent;
      parent = current->get_parent();
      
   }

   for(auto i = pathname.crbegin(); i != pathname.crend(); ++i){
      path += "/";
      path += *i;  
   }
   return path;
}
