// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $

/////////////////////////////////////
// Name: Camellia Boutros          //
// CruzID: cboutros                //
// Class: CMPS 109                 //
// Title: asg3                     //
// Date: 4/29/2016                 //
/////////////////////////////////////

//Using some of Professor Mackey's example code as a template

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string, string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

string trim (const string &str) {
   size_t first = str.find_first_not_of (" \t");
   if (first == string::npos) return "";
   size_t last = str.find_last_not_of (" \t");
   return str.substr (first, last - first + 1);
}

int parse(string cin_name, string line, int num, str_str_map& the_map){
   int linenum = num + 1;
   str_str_map::iterator itor;
   
   cout << endl << cin_name << ": " << linenum << ": " << line << endl;
   line = trim(line);
   
   //Begin parse: // 

   if (line.size() == 0 or line[0] == '#'){
      TRACE('t', "\nBlank line or #comment" << endl); //test
      
      return linenum;
   }
   size_t position = line.find_first_of("=");
   if(position == string::npos) {
      //not here test
      TRACE('t', "\nkey = \"" << line << "\"" << endl);
      TRACE('t', "\nSearch for keyvalue by key." << endl); //test 
      // Search for associated value by key 
      itor = the_map.find(line);
      if (itor != the_map.end()){//in list
         cout << (*itor).first << " = " << (*itor).second << endl; 
         // print key-value pair
      } else { 
         cout << line << ": key not found" << endl; //test
      }
   } else { 
       //here test
      string key = trim(line.substr(0, position == 0?0: position )); //posiiton - 1
      
      string value = trim(line.substr(position + 1));
      TRACE('t', "\nkey = \"" << key << "\"" << endl);
         
      TRACE('t', "value = \"" << value << "\"" << endl);
      if ((value == "") and (key != "")){ //"key = " 
         TRACE('t', "\nSearch by key; prep to delete." << endl);
         itor = the_map.find(key);
         if (itor != the_map.end()){
            TRACE('t', "Key found: prepare to delete" << endl); 
            the_map.erase(itor);           
         }  
         //else: ignore
      }else if ((key == "") and (value != "")) {
         TRACE('t', "\nSearch for keyvalue by value." << endl);
         itor = the_map.find_by_value(value);
         if (itor != the_map.end()){
            
            cout << (*itor).first << " = " << (*itor).second << endl; 
         }    
      } else if (key == "" and value == ""){
          TRACE('t', "\nDisplay all." << endl);
          the_map.display_all();
         //print all in lexicographical order
      } else { //insert
      
         str_str_pair pair (key, value);
         the_map.insert(pair);
      }
   }
   return linenum;
}

int main (int argc, char** argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);
   int linenum = 0;
   int updated_linenum;
   string cin_name = "-";
   //Initialize Listmap 
   str_str_map the_map;
   

   if (not (argc > 1) || ((argc == 3)and(strcmp(argv[1], "-@"))== 0)){
      for(;;){ //indent (strcmp(argv[1], "-@"))==0)   
          
         string line;
         getline(cin, line); 
         updated_linenum = parse(cin_name, line, linenum, the_map);
         linenum = updated_linenum;
         if(cin.eof()) break;
      }
   } else { //options specified
      int itor;
      string filename;
      if (strcmp(argv[1], "-@") == 0  ){ //(strcmp(argv[1], "-@")==0
         itor = 3; 
      }  else { 
         itor = 1;
      } 
      for (;itor != argc; ++itor){
         filename = argv[itor];
         cout << "Filename: " << filename << endl; //test
         //if (filename == cin_name) parse(cin, filename); //fix
         
         ifstream infile(filename);
         if (infile.fail()){
            cerr << "File failure" << endl;
         } else {
            for(;;) {
               string line;
               getline (infile, line);
               updated_linenum=parse(filename, line, linenum, the_map);
               linenum = updated_linenum;
               if (infile.eof()) break;
            } 
            infile.close();
         }
      } 
   }   
   
   


  
   //for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      
   //   cout << "Before insert: " << pair << endl;
   //   test.insert (pair);
   //}

   //for (str_str_map::iterator itor = test.begin();
   //     itor != test.end(); ++itor) {
   //   cout << "During iteration: " << *itor << endl;
   //}


   
   //str_str_map::iterator itor = test.begin();
   //test.erase (itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}





