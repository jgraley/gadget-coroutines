/**
 * @file Task.h
 * ### `gadget-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright @copyright (C) 2020 John Graley; BSD license applies.
 * 
 * @brief Task base class and interrupt handler macro.
 */
#ifndef Task_h
#define Task_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif


#include "Tracing.h"
#include "SuperFunctor.h"

#include <functional>

namespace GC
{

/**
 * @brief Base class for tasks.
 * 
 * We define a _task_ as a C++ object that can perform some useful work
 * by being invoked repeatedly and performiong a small amount of work
 * each time. The concept is already established in the Arduino 
 * ecosystem via the `loop()` function, which a sketch implements to 
 * perform a small amount of the task before returning, and will 
 * continue on the next call.
 * 
 * A task object is a functor: it implements `operator()` as the way
 * to invoke the task. This calls through to a protected pure virtual 
 * function that must be over-ridden in a concrete implementation. The
 * implementation should perform a small amount of work before 
 * returning.
 * 
 * This class also provides support for hopping. A hop lambda may be  
 * provided to the class and will be invoked exactly once, just before
 * the next (or current) invocation returns, at a safe time.
 * 
 * The reason for a separate Task class is that schedulers and the like
 * can work with `Task *`, rather than `Coroutine *` and will then be
 * be usable with non-coroutine code as well as coroutine code.
 */
class Task : public SuperFunctor
{
public:
  /**
   * Create an instance.
   */ 
  Task();

  /**
   * Destroy an instance.
   */ 
  virtual ~Task() = default;
    
  /**
   * Functor entry point: invoke the task.
   */ 
  virtual void operator()();
  
  /**
   * Give the task some code to be executed next time the functor returns.
   * 
   * Note that this code _is allowed_ to enable an interrupt whose
   * ISR may then _re-enter_ the task's functor interface. This is 
   * likely to happen when hopping from foreground to an interrupt that 
   * is already pending. `run_iteration()` will not be re-entered.
   * 
   * @param hop a lambda to be executed when the functor returns.
   */  
  inline void set_hop_lambda( std::function<void()> hop );
  
protected:
  inline void check_valid_this() const;
  virtual void run_iteration() = 0;
  
private:
  const uint32_t magic;
  std::function<void()> hop_lambda;

  static const uint32_t MAGIC;
};

// Implement the inline functions here

void Task::check_valid_this() const
{
  ASSERT( magic == MAGIC, "bad this pointer or object corrupted: %p", this );
}

void Task::set_hop_lambda( std::function<void()> hop )
{
    hop_lambda = hop;
}

} // namespace

// NOTE: if super functors are disabled, we should be able to change the 
// implementation here to use `Task &` and `Task *` directly. #43
#define INTERRUPT_HANDLER_IMPL(ISR_NAME, PTR, GET) \
void (*ISR_NAME##PTR)() = nullptr; \
void ISR_NAME() \
{ \
  if( ISR_NAME##PTR ) \
    ISR_NAME##PTR(); \
} \
void (**GET##ISR_NAME())() \
{ \
  return &ISR_NAME##PTR; \
}

/**
 * @brief Declare a re-directable interrupt handler.
 * 
 * An interrupt handler function will be generated with the given name
 * as well as a function named `get_ISRNAME()` that returns a pointer to 
 * a RAM-based interrupt vector. This can be used to re-direct the 
 * interrupt at run time.
 * 
 * @param ISR_NAME name of the interrupt to generate.
 */
#define INTERRUPT_HANDLER(ISR_NAME) \
  INTERRUPT_HANDLER_IMPL(ISR_NAME, _ptr, get_)

#endif
