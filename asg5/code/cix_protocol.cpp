#include <iostream>
using namespace std;
#include "cix_protocol.h"

string get_cix_client_port(const vector<string>& args, size_t index) {
   if (index < args.size()) return args[index];
   char* port = getenv ("CIX_CLIENT_PORT");
   if (port != nullptr) return port;
   return "localhost";
}
