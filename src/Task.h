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


#include "CoroTracing.h"

#ifdef ENABLE_SUPERFUNCTOR
// Super Functors work, but only really useful if the vector table is in RAM
#include "SuperFunctor.h"
#endif

#include <functional>

class Task 
#ifdef ENABLE_SUPERFUNCTOR
    : public SuperFunctor
#endif
{
public:
  Task();
  virtual ~Task() = default;
    
  virtual void operator()();
  
protected:
  inline void check_valid_this() const;
  virtual void run_iteration() = 0;
  inline void set_hop_lambda( std::function<void()> hop );
  
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

#endif
