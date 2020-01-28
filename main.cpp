// $Id: main.cpp,v 1.2 2019-12-12 19:22:40-08 - - $

#include <cassert>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
using namespace std;

#include <unistd.h>

#include "bigint.h"
#include "debug.h"
#include "iterstack.h"
#include "libfns.h"
#include "scanner.h"
#include "util.h"

using bigint_stack = iterstack<bigint>;

void do_arith (bigint_stack& stack, const char oper) {
   if (stack.size() < 2) throw ydc_error ("stack empty");
   bigint right = stack.top(); //cout << "debug right = " << right;
   stack.pop();
   DEBUGF ('d', "right = " << right);
   bigint left = stack.top();
   stack.pop();
   DEBUGF ('d', "left = " << left); //cout << "debug left= " << left;
   bigint result;
   switch (oper) {
      case '+': result = left + right; break; //done
      case '-': result = left - right; break; //done
      case '*': result = left * right; break; // in progress
      case '/': result = left / right; break;
      case '%': result = left % right; break;
      case '^': result = pow (left, right); break;
      default: throw invalid_argument ("do_arith operator "s + oper);
   }
   DEBUGF ('d', "result = " << result);
   //cout<< "pushing" << endl ;
   //result.makeNeg(left.isNeg()); //result sign stored in right 
   stack.push (result);
}

void do_clear (bigint_stack& stack, const char) {
   DEBUGF ('d', "");
   stack.clear();
}


void do_dup (bigint_stack& stack, const char) {
   bigint top = stack.top();
   DEBUGF ('d', top);
   stack.push (top);
}

void do_printall (bigint_stack& stack, const char) {
   for (const auto& elem: stack) cout << elem << endl;
}

void do_print (bigint_stack& stack, const char) {
   if (stack.size() < 1) throw ydc_error ("stack empty");
   cout << stack.top() << endl;
}

void do_debug (bigint_stack&, const char) {
   cout << "Y not implemented" << endl;
}

class ydc_quit: public exception {};
void do_quit (bigint_stack&, const char) {
   throw ydc_quit();
}

string unimplemented (char oper) {
   if (isgraph (oper)) {
      return "'"s + oper + "' ("s + octal (oper) + ") unimplemented";
   }else {
      return octal (oper) + " unimplemented"s;
   }
}

void do_function (bigint_stack& stack, const char oper) {
   switch (oper) {
      case '+': do_arith    (stack, oper); break;
      case '-': do_arith    (stack, oper); break;
      case '*': do_arith    (stack, oper); break;
      case '/': do_arith    (stack, oper); break;
      case '%': do_arith    (stack, oper); break;
      case '^': do_arith    (stack, oper); break;
      case 'Y': do_debug    (stack, oper); break;
      case 'c': do_clear    (stack, oper); break;
      case 'd': do_dup      (stack, oper); break;
      case 'f': do_printall (stack, oper); break;
      case 'p': do_print    (stack, oper); break;
      case 'q': do_quit     (stack, oper); break;
      default : throw ydc_error (unimplemented (oper));
   }
}


//
// scan_options
//    Options analysis:  The only option is -Dflags. 
//
void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            error() << "-" << static_cast<char> (optopt)
                    << ": invalid option" << endl;
            break;
      }
   }
   if (optind < argc) {
      error() << "operand not permitted" << endl;
   }
}


//
// Main function.
//
int main (int argc, char** argv) {
   exec::execname (argv[0]);
   scan_options (argc, argv);
   bigint_stack operand_stack;
   scanner input;
   try {
      for (;;) {
         try {
            token lexeme = input.scan();
            switch (lexeme.symbol) {
               case tsymbol::SCANEOF:
                  throw ydc_quit();
                  break;
               case tsymbol::NUMBER:
                  operand_stack.push (bigint (lexeme.lexinfo));
                  break;
               case tsymbol::OPERATOR: {
                  char oper = lexeme.lexinfo[0];
                  do_function (operand_stack, oper);
                  break;
                  }
               default:
                  assert (false);
            }
         }catch (ydc_error& error) {
            cout << exec::execname() << ": " << error.what() << endl;
         }
      }
   }catch (ydc_quit&) {
      // Intentionally left empty.
   }
   return exec::status();
}

