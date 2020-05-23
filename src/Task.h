/**
 * Task.h - Task interface
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Task interface class
 */
#ifndef Task_h
#define Task_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif


#include "Tracing.h"
#include "SuperFunctor.h"

#include <functional>

class Task : public SuperFunctor
{
public:
  Task();
  virtual ~Task() = default;
    
  virtual void operator()();
  inline void set_hop_lambda( std::function<void()> hop );
  
protected:
  inline void check_valid_this() const;
  virtual void run_iteration() = 0;
  
private:
  const uint32_t magic;
  std::function<void()> hop_lambda;

  static const uint32_t MAGIC;
};

///-- 
// Implement the inline functions here

void Task::check_valid_this() const
{
  ASSERT( magic == MAGIC, "bad this pointer or object corrupted: %p", this );
}

void Task::set_hop_lambda( std::function<void()> hop )
{
    hop_lambda = hop;
}

// NOTE: if super functors are disabled, we should be able to change the 
// implementation here to use `Task &` and `Task *` directly.
#define INTERRUPT_HANDLER_IMPL(ISR_NAME, PTR, ATTACH, DETACH) \
void (*ISR_NAME##PTR)() = nullptr; \
void ISR_NAME() \
{ \
  if( ISR_NAME##PTR ) \
    ISR_NAME##PTR(); \
} \
void ATTACH##ISR_NAME(void (*handler)()) \
{ \
    ISR_NAME##PTR = handler; \
} \
void DETACH##ISR_NAME() \
{ \
    ISR_NAME##PTR = nullptr; \
}

#define INTERRUPT_HANDLER(ISR_NAME) INTERRUPT_HANDLER_IMPL(ISR_NAME, _ptr, Attach_, Detach_)

#endif
