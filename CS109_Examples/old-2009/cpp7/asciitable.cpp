// $Id: asciitable.cpp,v 1.1 2012-06-05 22:12:27-07 - - $
//
// NAME
//    asciitable - print the ascii charset
//
// SYNOPSIS
//    asciitable
//
// DESCRIPTION
//    Prints the ascii charset in decimal, octal, and hexadecimal,
//    along with the character if it is printable.  This program
//    illustrates how to print numbers in different bases and with
//    leading zeros.
//

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <locale>

using namespace std;

const int rows = 32;
const int cols =  4;

string asciiname (unsigned char byte) {
   if (byte == 0x7F) return "DEL";
   static string asciiname[] = {
      "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
      "BS ", "HT ", "LF ", "VT ", "FF ", "CR ", "SO ", "SI ",
      "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
      "CAN", "EM ", "SUB", "ESC", "FS ", "GS ", "RS ", "US "};
   if (byte <= 0x3F) return asciiname[byte];
   return "???";
}

void printbyte (int width, char fill, int base, int byte) {
   cout.setf (ios_base::uppercase);
   cout << " " << setw (width) << setfill (fill)
        << setbase (base) << byte;
}

int main (int argc, char **argv) {
   for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
         int byte = row + col * rows;
         printbyte (4, ' ', 10, byte);
         printbyte (3, '0',  8, byte);
         printbyte (2, '0', 16, byte);
         if (isprint (byte)) {
            cout << " '" << static_cast<char>(byte) << "'";
         }else{
            cout << " " << asciiname (byte);
         };
      };
      cout << endl;
   };

   return EXIT_SUCCESS;
};

