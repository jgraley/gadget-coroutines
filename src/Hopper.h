/**
 * Hopper.h 
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Support for hopping
 */
#ifndef Hopper_h
#define Hopper_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif


#include "Tracing.h"
#include "Coroutine.h"

#include <functional>

class Hopper
{
public:
  Hopper( std::function<void()> &&attach_, std::function<void()> &&detach_ ) :
    previous_hop( current_hop ),
    attach( attach_ ),
    detach( detach_ )
  {
    if( previous_hop )
      previous_hop->detach();
    me()->set_hop_lambda( attach );
    current_hop = this;
  }

  void hop(std::function<void()> new_attach, std::function<void()> new_detach)
  {
    detach();
    attach = new_attach;
    detach = new_detach;
    me()->set_hop_lambda( attach );
  }

  ~Hopper()
  {
    current_hop = previous_hop;
    detach();
    if( previous_hop )
      me()->set_hop_lambda( previous_hop->attach );
  }
private: 
  Hopper * const previous_hop;
  static __thread Hopper *current_hop;

  std::function<void()> detach;                             
  std::function<void()> attach;
};

#endif
