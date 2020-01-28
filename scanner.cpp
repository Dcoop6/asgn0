// $Id: scanner.cpp,v 1.1 2019-12-12 18:19:23-08 - - $

#include <cassert>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
using namespace std;

#include "scanner.h"
#include "debug.h"

char scanner::get() {
   if (not good()) throw runtime_error ("scanner::get() past EOF"); 
   char currchar = nextchar;
   nextchar = instream.get();
   return currchar;
}

token scanner::scan() {
   while (good() and isspace (nextchar)) get();
   if (not good()) return {tsymbol::SCANEOF};
   if (nextchar == '_' or isdigit (nextchar)) {
      token result {tsymbol::NUMBER, {get()}};
      while (good() and isdigit (nextchar)) result.lexinfo += get();
      return result;
   }
   return {tsymbol::OPERATOR, {get()}};
}

ostream& operator<< (ostream& out, tsymbol symbol) {
   struct hasher {
      auto operator() (tsymbol sym) const {
         return static_cast<underlying_type<tsymbol>::type> (sym);
      }
   };
   static const unordered_map<tsymbol,string,hasher> map {
      {tsymbol::NUMBER  , "NUMBER"  },
      {tsymbol::OPERATOR, "OPERATOR"},
      {tsymbol::SCANEOF , "SCANEOF" },
   };
   return out << map.at(symbol);
}

ostream& operator<< (ostream& out, const token& token) {
   out << "{" << token.symbol << ", \"" << token.lexinfo << "\"}";
   return out;
}

