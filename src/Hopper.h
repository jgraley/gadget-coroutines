/**
 * @file Hopper.h 
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 * 
 * @brief Support for hopping
 */
#ifndef Hopper_h
#define Hopper_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include "Tracing.h"
#include "Coroutine.h"

#include <functional>

namespace HC
{
/**
 * @brief Utility class for hopping between contexts in RAII style
 * 
 * RAII stands for _Resource Acquisition Is Initialization_. In this 
 * programming style, we allocate resources in constructors and then 
 * release them in destructors. Then, as far as possible, C++ will try
 * to ensure the resource always gets released at the right time, which
 * is scope exit for automatic objects (non-static locals).
 * 
 * Here, the "resource" is receiving invocations from a particular 
 * source. The user proivides lambdas to actually obtain and release
 * that resource (i.e. to attach to the source and detach again) and 
 * the `Hopper` object will ensure that the detach lambda is called 
 * on scope exit (if allocated automatically).
 * 
 * More than one `Hopper` object may be created. Within any given 
 * coroutine, these must have strictly nested lifetimes (as they will if 
 * automatically allocated). Constructing a second `Hopper` object will 
 * detach the first; destructing the second will re-attach the first. 
 * This is useful for example to hop onto an idle loop context to do 
 * the majority of the processing work, but to occasionally hop onto 
 * a device interrupt to perfom time-critical I/O. When the function
 * returns, the default context is re-attached automatically.
 */ 
class Hopper
{
public:
  /**
   * Hopper constructor. Will detach from the context of any existing 
   * `Hopper` object in this coroutine, and then attach to the new context.
   * **Note that the attach_ lambda is not actually executed until the next 
   * yield operation.**
   * 
   * @param attach_ a lambda that is executed when hopping on to the context
   * @param detach_ a lambda that is executed when hopping off the context
   */ 
  Hopper( std::function<void()> &&attach_, std::function<void()> &&detach_ );
  
  /**
   * Hopper destructor. Will detach from this `Hopper` object's context, 
   * and then attach to the context of the prevous `Hopper` object's
   * context, if one existed. **Note that the attach_ lambda is not 
   * actually executed until the next yield operation.**
   */
~Hopper();
  
  /**
   * Perform a sideways hop. Change the current hop without needing to 
   * destruct the current `Hopper` object or create a new one. **Note 
   * that the attach_ lambda is not actually executed until the next 
   * yield operation.**
   * 
   * @param new_attach a lambda that is executed when hopping on to the context
   * @param new_detach a lambda that is executed when hopping off the context
   */ 
  void hop(std::function<void()> &&new_attach, std::function<void()> &&new_detach);

private: 
  Hopper * const previous_hop;
  static __thread Hopper *current_hop;

  std::function<void()> detach;                             
  std::function<void()> attach;
};

} // namespace

#endif
