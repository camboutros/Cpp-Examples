// $Id: commands.cpp,v 1.16 2016-01-14 16:10:40-08 - - $

/////////////////////////////////////
// Name: Camellia Boutros          //
// CruzID: cboutros                //
// Class: CMPS 109                 //
// Title: asg2                     //
// Date: 4/13/2016                 //
/////////////////////////////////////

#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

void fn_cat (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1){
      throw command_error("Error: must specify a pathname");
   } else if (words.size() == 2){
      wordvec pathname;
      pathname = split(words.at(1), "/");
      if (pathname.size() == 1){ //only name of file given
         inode_ptr current = state.get_current();
         inode_ptr file;
         try {
            file = current->find_inode(words.at(1));
            file->readfile();
         }
         catch(file_error){
            cout << "cat: " << words.at(1) << ": No such file" << endl;
         }
      } else {
         string name = *(pathname.end() - 1);
         string actual_path;
         for (auto i = pathname.begin(); i != (pathname.end()-1); ++i){ 
            actual_path += *i;
            actual_path += "/";
         }
         
         wordvec new_path;
         new_path.push_back("first_arg");
         new_path.push_back(actual_path);
         fn_cd(state, new_path);
         inode_ptr current = state.get_current();
         inode_ptr file;
         try{
            file = current->find_inode(name);
            file->readfile();
         }
         catch(file_error){
            cout << "cat: " << words.at(1) << ": No such file" << endl;
         }
      }
   } else if (words.size() > 2){
      throw command_error("Too many args");
   }
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
 
   
   if (words.size() == 1) { //no args, cd to root 
      inode_ptr root_ptr = state.get_root();

      DEBUGF('c', "Root ptr value: " << root_ptr << endl);
      state.set_current(root_ptr);
      
      
   } 
   if (words.size() == 2) { //1 arg
      wordvec pathname = split(words.at(1), "/");
      for (auto i = pathname.cbegin(); i != pathname.cend(); ++i){
         inode_ptr current_ptr = state.get_current();
         inode_ptr next_inode {nullptr};
         try{
            next_inode = current_ptr->find_inode(*i);
         } 
         catch (file_error){
            throw command_error ("Error: directory not found");
         }
         if (next_inode->f_type != "plain" ) { //is a directory type 
            state.set_current(next_inode);
         } else {
            throw command_error("Not a directory"); 
         } 
      }
   }
   if (words.size() > 2 ) {
      throw command_error("Too many args");
   }
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
  
   //state.get_root()->remove_rec();
 
   throw ysh_exit();
} 

void fn_ls (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1){ // no pathname specified 
    
      inode_ptr i_on_display = state.get_current(); //set to cwd
      int i_nr = i_on_display-> get_inode_nr();
      size_t size = i_on_display->get_inode_size();
      inode_ptr parent;
      ///////// Begin long complicated procedure
      wordvec annoying_parent;
      annoying_parent.push_back("first_arg");
      annoying_parent.push_back("..");
      fn_cd(state, annoying_parent);
      parent = state.get_current();
        
      string name = parent->dad_what_my_name(i_on_display);      
      wordvec return_path;
      return_path.push_back("first_arg");
      return_path.push_back(name); //back into original current 
      fn_cd(state, return_path);  
      ////// End long complicated procedure
      /////// For display name: //////////////
      if(state.get_current() == state.get_root()) {
         cout << "\n\n/:" << endl;
      } else {
         cout << "\n\n" << name << ":"  << endl;
      }
      i_on_display->display_contents(i_nr, size, name);
       
   } else if (words.size() == 2){ //pathname specified   
      try{
      wordvec pathname;
      pathname = split(words.at(1), "/");
      string i_name;
      if (pathname.size() == 0){
         pathname.push_back(".");
         i_name = ".";
      } else if (pathname.size() > 1){
         i_name = (*(pathname.cend()-1));
      } else if (pathname.size() == 1){
         
         i_name = (pathname.at(0));
      } 
      //cout << "Name: " << i_name << endl;//test
      wordvec actual_pathname;
      string string_pathname;
      actual_pathname.push_back("first_arg");
      for (auto i = pathname.cbegin(); i != (pathname.cend() - 1); ++i){
         string_pathname += *i + "/";
      }
      
      actual_pathname.push_back(string_pathname);
      
         fn_cd(state, actual_pathname);
      
      
      inode_ptr current = state.get_current();
   
      inode_ptr i_on_display = current->find_inode(i_name);
      int i_nr = i_on_display-> get_inode_nr();
      size_t size = i_on_display->get_inode_size();
      inode_ptr parent;

      ///// For display name: ////////////////////      
      if(state.get_current() == state.get_root()) {
         cout << "\n\n/:" << endl;
      } else {
         cout << "\n\n" << i_name << ":" << endl;
      }
       
      i_on_display->display_contents(i_nr, size, i_name); 

      /////// Get back to original directory... ///
      wordvec return_path;
      return_path.push_back("first_arg");
      return_path.push_back("..");
      for (auto i = pathname.cbegin(); i != (pathname.cend()-1); i++){
         fn_cd(state, return_path);
      }
   } 
   catch (file_error e ) {
      cout << "error" << endl; 
   }
   } else if (words.size() > 2) {
      throw command_error("Too many args");
   }
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1){//no args given: print entire tree
      inode_ptr i_on_display = state.get_current();
      
      int i_nr = i_on_display-> get_inode_nr();
      size_t size = i_on_display->get_inode_size();
    
      inode_ptr parent;
      ///////// Begin long complicated procedure
      wordvec annoying_parent;
      annoying_parent.push_back("first_arg");
      annoying_parent.push_back("..");
      fn_cd(state, annoying_parent);
      parent = state.get_current();
        
      string name = parent->dad_what_my_name(i_on_display);
      wordvec return_path;
      return_path.push_back("first_arg");
      return_path.push_back(name); //back into original current 
      fn_cd(state, return_path);  
      ////// End long complicated procedure
      ////// For display name: //////////// 
      if(state.get_current() == state.get_root()) {
         cout << "\n\n/:" << endl;
      } 
      i_on_display->display_all(i_nr, size, name);
   } else if (words.size() == 2) {//pathname specified
      
      wordvec pathname;
      pathname = split(words.at(1), "/");
      string i_name;
      if (pathname.size() == 0){
         pathname.push_back(".");
         i_name = ".";
      } else if (pathname.size() > 1){
         i_name = (*(pathname.cend()-1));
      } else if (pathname.size() ==1){
         i_name = pathname.at(0);
      }
      //cout << "Name: " << i_name << endl;//test
      wordvec actual_pathname;
      string string_pathname;
      actual_pathname.push_back("first_arg");
      for (auto i = pathname.cbegin(); i != (pathname.cend() - 1); ++i){
         string_pathname += *i + "/";
      }
      
      actual_pathname.push_back(string_pathname);
      try {
         fn_cd(state, actual_pathname);
      } 
      catch(file_error e){
         throw command_error("Error: Directory not found");
      }
      inode_ptr current = state.get_current();
      try {
         inode_ptr i_on_display = current->find_inode(i_name);
         int i_nr = i_on_display-> get_inode_nr();
         size_t size = i_on_display->get_inode_size();
         inode_ptr parent; 
         ////////// For display name: ///////////// 
         if(state.get_current() == state.get_root()) {
            cout << "\n\n/:" << endl;
         } 
         i_on_display->display_all(i_nr, size, i_name); 
   
         /////// Get back to original directory... ///
      }
      catch (file_error e){
         throw command_error("Error: directory not found"); 
      }
      wordvec return_path;
      return_path.push_back("first_arg");
      return_path.push_back("..");
      for (auto i = pathname.cbegin(); i != (pathname.cend()-1); i++){
         fn_cd(state, return_path);
      }
   } else if (words.size() > 2){
      throw command_error("Too many args");
   } 
}

void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 1) {
      cout << "Error: must specify a filename." << endl;
   } else if (words.size() == 2){
      inode_ptr current_inode = state.get_current();
      current_inode->mkfile(words.at(1));
   } else if (words.size() == 3){ 
      inode_ptr home = state.get_current();
      
      wordvec pathname;
      pathname.push_back("first_arg"); //placeholder value
      pathname.push_back(words.at(2)); //holds string pathname
      
      fn_cd (state, pathname);
      inode_ptr temp_current = state.get_current();
      temp_current->mkfile(words.at(1));
      
      ////////Return to original directory//////////
      //string return_path;
      //wordvec return_pathname;
      //
      
      //   string name = *i; 
      //   inode_ptr next;
      //   next = temp_current->find_inode(name);
      //   return_path += name;
      //   return_path += "/";
      //   temp_current = next;
      //}
      //inode_ptr parent = home->get_parent(); 
      //string lastname = parent->dad_what_my_name(home); 
      //return_path += lastname;
      //return_pathname.push_back("first_arg");
      //return_pathname.push_back(return_path);
      //fn_cd(state, return_pathname);
      //cout << "return path: " << return_path << endl; //test
      /////////////////////////////////////////////
   } else {
      
      inode_ptr current = state.get_current();
      wordvec ball_of_yarn; //way back to current dir

     
      wordvec pathname;
      pathname.push_back("first_arg"); //placeholder value
      pathname.push_back(words.at(2)); //holds string pathname
      fn_cd (state, pathname);
      inode_ptr current_inode = state.get_current();
      current_inode->mkfile(words.at(1));

      wordvec file_words;
      for (auto i = words.cbegin() + 3; i != words.cend(); ++i){
         file_words.push_back(*i);
      }
      inode_ptr file;
      file = current_inode->find_inode(words.at(1)); 
      file->writefile(file_words);
      ///////// Attempt to return in the works
      ///
      ///wordvec split_pathname;
      ///split_pathname = split(words.at(2), "/");
      
         ///wordvec ball_of_yarn;
         ///ball_of_yarn.push_back("first_arg");
         ///ball_of_yarn.push_back(*i);
         ///fn_cd (state, ball_of_yarn); 
      ///}
   }    
}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   if (words.size() == 1) {
      throw command_error("Error: must specify a pathname."); 
   } else if (words.size() == 2){ 
      inode_ptr current_inode = state.get_current();
      current_inode->mkdir(words.at(1)); 
   } else if (words.size() == 3){
      inode_ptr current = state.get_current();
      wordvec ball_of_yarn; //way back to current dir
      ball_of_yarn.push_back("first_arg");
      ball_of_yarn.push_back(".."); //hopping back by parent
      wordvec pathname;
      pathname.push_back("first_arg"); //placeholder value
      pathname.push_back(words.at(2)); //holds string pathname
      fn_cd (state, pathname);
      inode_ptr current_inode = state.get_current();
      current_inode->mkdir(words.at(1));
      wordvec split_pathname;
      split_pathname = split(words.at(2), "/");
      for (auto i=split_pathname.begin();i!=split_pathname.end();++i){
         fn_cd (state, ball_of_yarn); 
      }
   } else {
      throw command_error( "Too many args"); 
   }
}

void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   string new_prompt;
   if (words.size() == 1){
        cout << "No prompt symbol specified" << endl; 
        new_prompt = "";
   } else {
       for (auto i = (words.cbegin() + 1); i != words.cend(); ++i){
          new_prompt = new_prompt + *i; 
       }
   }
   state.set_prompt((new_prompt + " "));
   cout << "\nNew prompt is: " << state.prompt() << endl;//test
}

void fn_pwd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   if (words.size() > 1) {
      throw command_error("Too many args");
   }
   string path;
   if (state.get_current() == state.get_root()) {
      path += "/";
   } else {
      inode_ptr current = state.get_current();
      path = current->pwd(); 
      
   }  
   cout << path << endl;
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   if (words.size() == 1){ // no pathname specified
      throw command_error("Must specify pathname");
   } else if (words.size() > 2) {
      throw command_error("Too many args");
   } else if (words.size() == 2){
      string pathname = words.at(1);
      
      wordvec split_pn;
      split_pn = split(pathname, "/"); //unaltered
      string to_rm = *(split_pn.cend()-1); //inode name
      wordvec split_pn_shrt; //altered
      string pname_shrt;
      
      for (auto i = split_pn.cbegin(); i != (split_pn.cend() - 1); ++i){
         pname_shrt += *i; 
         pname_shrt += "/";
      }
      
      split_pn_shrt.push_back("first_arg");
      split_pn_shrt.push_back(pname_shrt);
      try{
         fn_cd(state, split_pn_shrt);
         inode_ptr current = state.get_current();
         current->remove(to_rm);
      } 
      catch (file_error e){
         cout << "fn_rm: Directory not found" << endl;
     }
   } 
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   if (words.size() == 1){ // no pathname specified
      throw command_error("Must specify pathname");
   } else if (words.size() > 2) {
      throw command_error("Too many args");
   } else if (words.size() == 2){
      
      string pathname = words.at(1);
      
      wordvec split_pn;
      split_pn = split(pathname, "/"); //unaltered
      string to_rm = *(split_pn.cend()-1); //inode name
      wordvec split_pn_shrt; //altered
      string pname_shrt;
      
      for (auto i = split_pn.cbegin(); i != (split_pn.cend() - 1); ++i){
         pname_shrt += *i; 
         pname_shrt += "/";
      }
      
      split_pn_shrt.push_back("first_arg");
      split_pn_shrt.push_back(pname_shrt);
      
     // try{ 
         try {
            fn_cd(state, split_pn_shrt);
            inode_ptr current = state.get_current();
            inode_ptr to_destroy = current->find_inode(to_rm);

            
            to_destroy->remove_rec();
         } 
         catch (file_error e){
            cout << "fn_rm: Directory not found" << endl;
         }
     // }
      //catch (const bad_alloc&){
       //  cout << "Bad alloc" << endl;
     // }
   }
}

