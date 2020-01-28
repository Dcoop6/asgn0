// $Id: iterstack.h,v 1.1 2019-12-12 18:19:23-08 - - $

// 
// The class std::stack does not provide an iterator, which is
// needed for this class.  So, like std::stack, class iterstack
// is implemented on top of a container.
// 
// We use private inheritance because we want to restrict
// operations only to those few that are approved.  All functions
// are merely inherited from the container, with only ones needed
// being exported as public.
// 
// No implementation file is needed because all functions are
// inherited, and the convenience functions that are added are
// trivial, and so can be inline.
//
// Any underlying container which supports the necessary operations
// could be used, such as vector, list, or deque.
// 

#ifndef __ITERSTACK_H__
#define __ITERSTACK_H__

#include <vector>
using namespace std;

template <typename value_t, typename container = vector<value_t>>
class iterstack {
   public:
      using value_type = value_t;
      using const_iterator = typename container::const_reverse_iterator;
      using size_type = typename container::size_type;
   private:
      container stack;
   public:
      void clear() { stack.clear(); }
      bool empty() const { return stack.empty(); }
      size_type size() const { return stack.size(); }
      const_iterator begin() { return stack.crbegin(); }
      const_iterator end() { return stack.crend(); }
      void push (const value_type& value) { stack.push_back (value); }
      void pop() { stack.pop_back(); }
      const value_type& top() const { return stack.back(); }
};

#endif

