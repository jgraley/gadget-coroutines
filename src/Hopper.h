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
  Hopper( std::function<void()> ext_detach_, std::function<void()> my_attach_, std::function<void()> my_detach_, std::function<void()> ext_attach_ ) :
    my_detach( my_detach_ ),
    ext_attach( ext_attach_ )
  {
    ext_detach_();
    me()->set_hop_lambda( my_attach_ );
  }

  void hop(std::function<void()> my_attach_, std::function<void()> my_detach_)
  {
    my_detach();
    me()->set_hop_lambda( my_attach_ );
    my_detach = my_detach_;
  }

  ~Hopper()
  {
    my_detach();
    me()->set_hop_lambda( ext_attach );
  }
private: 
  std::function<void()> my_detach;                             
  std::function<void()> ext_attach;
};

#endif
