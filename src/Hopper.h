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
  Hopper( std::function<void()> &&attach_, std::function<void()> &&detach_ );
  ~Hopper();
  
  void hop(std::function<void()> &&new_attach, std::function<void()> &&new_detach);

private: 
  Hopper * const previous_hop;
  static __thread Hopper *current_hop;

  std::function<void()> detach;                             
  std::function<void()> attach;
};

#endif
