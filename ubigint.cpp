// $Id: ubigint.cpp,v 1.8 2020-01-06 13:39:55-08 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <sstream>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (unsigned long that): uvalue (that) {
   DEBUGF ('~', this << " -> " << uvalue)
   int place = 1;
   string s;
   stringstream ss; 
   ss << that;
   s = ss.str();
   ubigint result(s);
   *this = result;
}
ubigint::ubigint (const ubigint& that) {
   *this = *this + that;
}
ubigint::ubigint (const string& that): uvalue(0) {
   DEBUGF ('~', "that = \"" << that << "\"");
   for (char digit: that) {
      if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
      num.push_back(digit - '0');
      uvalue = uvalue * 10 + digit - '0';
   }
}

ubigint ubigint::operator+ (const ubigint& that) const {
   int carry = 0;
   int sum = 0;
   ubigint result;
   vector<int> temp;
   auto LH = this->num.rbegin();
   auto RH = that.num.rbegin();

   while(RH != that.num.rend() && LH != this->num.rend()){
       sum = *RH + *LH;
       sum += carry;  
       carry = sum / 10;
       temp.push_back(sum%10);
       sum = 0;
       ++LH;
       ++RH;
   }
   while(LH != this->num.rend()){
       sum = *LH;
       sum += carry;  
       carry = sum / 10;
       temp.push_back(sum%10);
       //result.num.push_back(sum%10);
       sum = 0;
       ++LH;       
   }
   while(RH != that.num.rend()){
       sum = *RH;
       sum += carry;  
       carry = sum / 10;
       temp.push_back(sum%10);
       //result.num.push_back(sum%10);
       sum = 0;
       ++RH;
   
   }
   if (carry >0)
     temp.push_back(carry);
   //result.num.push_back(carry); 
   //return ubigint (uvalue + that.uvalue);
   while(!temp.empty()){
      result.num.push_back(temp.back());
      temp.pop_back();
   }

   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
   int carry = 0;
   int diff = 0;
   ubigint result;
   vector<int> temp;
   bool swap = false;
   if(*this > that)
     swap = true;

   auto RH = this->num.rbegin();
   auto LH = that.num.rbegin();

   while(LH != that.num.rend() && RH != this->num.rend()){
      if(!swap)
          diff =*LH - *RH;
      else 
          diff =*RH - *LH;
      diff += carry;
      if(diff < 0){
          diff = 10 + diff;
          carry = - 1;
      }
      else
          carry = 0; 
      temp.push_back(diff%10);
      diff = 0;
      ++LH;
      ++RH;
   }
   while(LH != that.num.rend() ){ 
      diff =*LH;
      diff += carry;
      if(diff < 0){
          diff = 10 + diff;
          carry = - 1;
      }
      else
          carry = 0;  
      temp.push_back(diff%10);
      diff = 0;
      ++LH;
   }
   while(RH != this->num.rend()){ 
      diff = *RH ;
      diff += carry;
      if(diff < 0){
          diff = 10 + diff;
          carry = - 1;
      }
      else
          carry = 0;  
      temp.push_back(diff%10);
      diff = 0;
      ++RH;
   }
   while(!temp.empty()){
      result.num.push_back(temp.back());
      temp.pop_back();
   }

   return result;
}

ubigint ubigint::operator* (const ubigint& that) const {
   //return ubigint (uvalue * that.uvalue);
   int carry = 0;
   int p = 0;
   ubigint result;
   ubigint temp;
   vector<int> t;
   auto LH = this->num.rbegin();
   auto RH = that.num.rbegin();
   int count = 0;
   bool cu = false;

   while(RH != that.num.rend())
   {

      for(int i = 0 ; i < count; i++)
          t.push_back(0);
  
      while(LH != this->num.rend()){

         p = (*LH) * (*RH);
         p += carry;
         carry =  p /10;
         t.push_back(p%10);
         p = 0;
         ++LH;
      }  
      LH = this->num.rbegin();
      ++RH;
      if(carry >0)
        t.push_back(carry);
      carry = 0;
      while(!t.empty()){
        temp.num.push_back(t.back());
        t.pop_back();
      }

      result = result + temp;
      temp.num.clear();
      t.clear();
      ++count;
   }
   return result;
}

void ubigint::multiply_by_2() {
   //uvalue *= 2;
   ubigint TWO(2);
   *this = *this * TWO;

   //cout << "In mult by 2 :" << *this << endl; 
}

void ubigint::divide_by_2() {
   ubigint two(2);
   ubigint sub2(1);
   ubigint sub1(1);

   auto RH = this->num.rbegin();
   while(RH != this->num.rend()){
      if(*RH % 2 == 1)
         *(RH-1) += 5; 
      *RH /= 2;
      ++RH;
   }
   
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, const ubigint& divisor_) {
   // NOTE: udivide is a non-member function.
   ubigint divisor {divisor_};
   ubigint zero {0};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {1};
   ubigint quotient {0};
   ubigint remainder {dividend}; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) { 
      if (divisor <= remainder) { 
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2(); 
   }
   
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   
   return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {

     auto LH = this->num.begin();
     auto RH = that.num.begin(); 
 
     while(LH != this->num.end() && RH != that.num.end()){ 
         if(*LH != *RH){ //cout<< "< opp true 2" << that << endl;
            return false;}
         ++LH;
         ++RH;
   //   }
   }
   if(LH != this->num.end() || RH != that.num.end())
       return false;
   return true;
   //return uvalue == that.uvalue;
}

bool ubigint::operator< (const ubigint& that) const {
   if (*this == that)
      return false;
   bool smaller = false;
   bool bigger = false;
   auto LH = this->num.begin();
   auto RH = that.num.begin();
   
   while(*LH == 0 && LH != this->num.end())
       ++LH;
   while(*RH == 0 && RH != that.num.end())
       ++RH;
   
   //if (num.size() < that.num.size()){cout << "true 1" << endl;
   //     return true; }

   while(LH != this->num.end() && RH != that.num.end()){
       if (*LH < *RH && !bigger)
           smaller = true;
       if (*LH > *RH && !smaller)
           bigger = true;
     //else if (*LH > *RH)
       //return false;
   
      ++LH;
      ++RH;
   
   }
   if(LH != this->num.end() && RH == that.num.end()){
        return false;
   }
   if(LH == this->num.end() && RH != that.num.end()){
        return true;
   }

   return smaller;
}

ostream& operator<< (ostream& out, const ubigint& that) { 
   //return out  << that.uvalue ;
   string newling = "\\\n";
   string output = "";
   bool leadZ = true;
   int c ;
   int j = 0;
   for(auto i = that.num.begin(); i != that.num.end(); i++)
   {
       
       if(j % 69 == 0 && j != 0)
          output+=newling;
       c = *i ;
       if(c +'0' != '0' && leadZ != false)
         leadZ = false;
       if(leadZ == false)
         output += c +'0';
       j++;
   }
   if(leadZ == true)
     output += c +'0';
   return out  << output ;
}

