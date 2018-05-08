// $Id: cix.cpp,v 1.4 2016-05-09 16:01:56-07 - - $

//////////////////////////////////////////////////
// Name: Camellia Boutros                       //
// CruzID: cboutros                             //
// Class: CMPS109                               //
// Title: Asg 5 Client and Server               //
// Date: 5/31/2016                              //
//////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cerrno>
#include <sstream>

using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "cix_protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", cix_command::EXIT},
   {"get",  cix_command::GET},
   {"help", cix_command::HELP},
   {"ls"  , cix_command::LS  },
   {"put",  cix_command::PUT},
   {"rm",   cix_command::RM},
};

void cix_get(client_socket &serv, string fname){ 
   if (FILENAME_SIZE <= fname.size()){
      log << "'" << fname << "' is too long of a name" << endl; 
      return;
   }
   cix_header head;
   head.command = cix_command::GET;
   strcpy(head.filename, fname.c_str());
   log << "Sending header " << head << endl;
   send_packet(serv, &head, sizeof head);
   recv_packet(serv, &head, sizeof head);
   log << "Received header " << head << endl;
   if (head.command != cix_command::FILE){
      log << "Sent command, server did not return FILE" << endl;
      log << "Server returned " << head << endl;
      log << strerror(head.nbytes) << endl;
   } else {
      ofstream file_stream{fname};
      char buffer[head.nbytes + 1]; 
      recv_packet(serv, buffer, head.nbytes);
      log << "Received " << head.nbytes << " bytes" << endl;
      buffer[head.nbytes] = '\0'; 
      file_stream.write(buffer, head.nbytes);
      file_stream.close();
      log << "Get: Success" << endl;
   }
}  



void cix_help() {
   static const vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = cix_command::LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::LSOUT) {
      log << "sent LS, server did not return LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

void cix_rm(client_socket &serv, string fname){
   if (FILENAME_SIZE <= fname.size()){
      log << "'" << fname << "' is too long of a name" << endl; 
      return;
   }
   cix_header head;
   head.command = cix_command::RM;
   head.nbytes = 0;
   strcpy(head.filename, fname.c_str()); //find a better method
   log << "Sending header " << head << endl;
   send_packet(serv, &head, sizeof head);
   recv_packet(serv, &head, sizeof head);
   log << "Received header " << head << endl;
   if(head.command != cix_command::ACK){ //double check protocol
      log << "Sent command, no ACK return from server" << endl;
      log << "Returned: " << head << endl;
      log << strerror(head.nbytes) << endl; 
      return;
   }
   log << "Remove: Success" << endl;
}


void cix_put(client_socket &serv, string fname){
   if (FILENAME_SIZE <= fname.size()){
      log << "'" << fname << "' is too long of a name" << endl; 
      return;
   }
   cix_header head;
   head.command = cix_command::PUT;
   ifstream file_stream{fname};
   if(!file_stream.good()){
      log << strerror(errno) << endl;
      return;
   } else {
      stringstream s;
      s << file_stream.rdbuf();
      string byte = s.str();
      strcpy(head.filename, fname.c_str());
      head.nbytes = byte.size();
      log << "Sending header " << head << endl;
      send_packet(serv, &head, sizeof head);
      send_packet(serv, byte.c_str(), byte.size());
      recv_packet(serv, &head, sizeof head);
      log << "Received header " << head << endl;
      if (head.command != cix_command::ACK){
         log << "Sent command, no ACK return from server" << endl;
         log << "Returned: " << head << endl;
         log << strerror(head.nbytes) << endl; 
         return;
      }
      log << "Put: Success" << endl; 
   }
}
      

void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

string trim (const string &to_trim){
   size_t pos = to_trim.find_first_not_of(" \t");
   if(pos == string::npos) {
      return "";
   } else {
      size_t end_pos = to_trim.find_last_not_of(" \t");
      return to_trim.substr(pos, ((end_pos - pos) +1));
   }
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   //
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         bool too_many = false; //fix test
         bool loop_back = false; // double check
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         log << "command " << line << endl;
         string input;
         input = trim(line);
         string format_cmd;
         string fname;
         if (input.size() == 0){
            continue;
         }
         size_t npos = string::npos;
         size_t position = input.find(" ");
         if (position != npos){
            format_cmd = input.substr(0, position);
            size_t cmd_pos = input.find(" ", position+1);
            if (cmd_pos == npos){
               fname = input.substr(position + 1);
               if (fname.find("/") != npos){
                  log << "Slashes not allowed in filenames" << endl;
                  continue;
               }
             } else {
               too_many = true;
             }
         }
       
         const auto& itor = command_map.find (line);
         cix_command cmd = itor == command_map.end()
                         ? cix_command::ERROR : itor->second;
         //cout << line << endl; //test
         switch (cmd) {
            case cix_command::EXIT:
               if (fname.size() > 0){
                  log << "Usage: exit" << endl;
                  continue;
               }
               throw cix_exit();
               break;
            case cix_command::GET:

               if (fname.size() > 0){
                  log << "Usage: exit" << endl;
                  continue;
               }
               log << "Getting file: " << fname << endl; 
               cix_get(server, fname);
               break;
            case cix_command::PUT:
               if (fname.size() > 0){
                  log << "Usage: put" << endl;
                  continue;
               }
               //Might remove log commands
               log << "Putting file: " << fname << endl; 
               cix_put(server, fname);
               break;
            case cix_command::HELP:
               if (fname.size() > 0){
                  log << "Usage: help" << endl;
                  continue;
               }
               cix_help();
               break;
            case cix_command::LS:
               if (fname.size() > 0){
                  log << "Usage: ls " << endl;
                  continue;
               }
               cix_ls (server);
               break;
            case cix_command::RM:
               if (fname.size() > 0){
                  log << "Usage: rm 'name'" << endl;
                  continue;
               }
                  log << "Removing: " << fname << endl; 
               cix_rm(server, fname);
               break;
            default:
               log << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}
/////////////////////////////////
// Found code by Brad Bernard  //
// to help me figure out       //
// proper ifstream syntax -    //
// on line 139                 //
/////////////////////////////////
