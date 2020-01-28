// $Id: bigint.cpp,v 1.2 2020-01-06 13:39:55-08 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue_, bool is_negative_):
                uvalue(uvalue_), is_negative(is_negative_) {

}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
   return *this;
}

bigint bigint::operator- () const {
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
  ubigint result;
  bigint result2;
  if (!is_negative && !that.is_negative){
      result = uvalue + that.uvalue;
      result2 = bigint(result,false);
     
  }
  if (!is_negative && that.is_negative){
      result = uvalue - that.uvalue;

      if(uvalue > that.uvalue)
        result2 = bigint(result, false);
      else
        result2 = bigint(result, true);
  }
  if (is_negative && !that.is_negative){
      result = that.uvalue - uvalue;

      if(uvalue > that.uvalue)
        result2 = bigint(result, true);
      else
        result2 = bigint(result, false);
  }
  if (is_negative && that.is_negative){
      result = uvalue + that.uvalue;
      result2 = bigint(result,true);
  }
  return result2;
}

bigint bigint::operator- (const bigint& that) const {
  ubigint result;
  bigint result2;

  if (!is_negative && !that.is_negative){ // x - y
      result = uvalue - that.uvalue;

      if(uvalue < that.uvalue){
          result2 = bigint(result,true);
      }
      else
        result2 = bigint(result,false);
     
  }
  if (!is_negative && that.is_negative){ // x +y 
      result = uvalue + that.uvalue;

      result2 = bigint(result, false);
  }
  if (is_negative && !that.is_negative){ // -x -y
      result = that.uvalue + uvalue;

      result2 = bigint(result, true);
  }
  if (is_negative && that.is_negative){  // -x + y

      result = uvalue - that.uvalue;
      if(uvalue > that.uvalue)
        result2 = bigint(result,true);
      else
        result2 = bigint(result,false);
  }
  return result2;
}


bigint bigint::operator* (const bigint& that) const {
   ubigint result;
   bigint result2;
   result = that.uvalue * uvalue;
   if((!is_negative && that.is_negative) ||
     (is_negative && !that.is_negative))
       result2 = bigint(result,true);
   else
       result2 = bigint(result,false);
    return result2;
}

bigint bigint::operator/ (const bigint& that) const {
   bigint result = uvalue / that.uvalue;
   return result;
}

bigint bigint::operator% (const bigint& that) const {
   bigint result = uvalue % that.uvalue;
   return result;
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
   return is_negative ? uvalue > that.uvalue
                      : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   return out << (that.is_negative ? "-":"")
              << that.uvalue;
}
void bigint::makeNeg(bool sign) {
    is_negative = sign;
}
bool bigint::isNeg() {
    return is_negative;
}

