
// $Id: protocol.h,v 1.3 2016-05-09 16:01:56-07 - - $

//////////////////////////////////////////////////
// Name: Camellia Boutros                       //
// CruzID: cboutros                             //
// Class: CMPS109                               //
// Title: Asg 5 Client and Server               //
// Date: 5/31/2016                              //
//////////////////////////////////////////////////

#ifndef __CIX_PROTOCOL__H__
#define __CIX_PROTOCOL__H__

#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;

#include "sockets.h"
#include "cix_protocol.h"

string get_cix_client_port (const vector<string>& args, size_t index);
#endif
