// $Id: util.h,v 1.2 2019-12-12 19:22:40-08 - - $

//
// util -
//    A utility class to provide various services
//    not conveniently included in other modules.
//

#ifndef __UTIL_H__
#define __UTIL_H__

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

#include "debug.h"

//
// ydc_error -
//    Indicate a problem where processing should be abandoned and
//    the main function should take control.
//

class ydc_error: public runtime_error {
   public:
      explicit ydc_error (const string& what): runtime_error (what) {
      }
};

//
// octal -
//    Convert integer to octal string.
//

template <typename numeric>
const string octal (numeric number) {
   ostringstream stream;
   stream << showbase << oct << (number + 0);
   return stream.str();
}


//
// main -
//    Keep track of execname and exit status.  Must be initialized
//    as the first thing done inside main.  Main should call:
//       main::execname (argv[0]);
//    before anything else.
//

class exec {
   private:
      static string execname_;
      static int status_;
      static void execname (const string& argv0);
      friend int main (int, char**);
   public:
      static void status (int status);
      static const string& execname() {return execname_; }
      static int status() {return status_; }
};

//
// complain -
//    Used for starting error messages.  Sets the exit status to
//    EXIT_FAILURE, writes the program name to cerr, and then
//    returns the cerr ostream.  Example:
//       complain() << filename << ": some problem" << endl;
//

ostream& note();
ostream& error();

#endif

