/**
 * Task.cpp - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 */

#include "Task.h"

#include <functional>
#include <atomic>

using namespace std;

Task::Task() :
  magic( MAGIC )
{
}


void Task::operator()()
{
  check_valid_this();

  run_iteration();
    
  if( !hop_lambda )
    return;
  auto local_hop_lambda = move(hop_lambda);
  hop_lambda = std::function<void()>(); // clear it

  // This will cause re-entry if a higher priority interrupt is enabled
  // than whatever is runnig us now. It's OK as long as we leave it at 
  // bottom of the function.
  atomic_thread_fence(memory_order_release);
  local_hop_lambda();    
}


constexpr uint32_t make_magic_le(const char *str)
{
  return str[0] | (str[1]<<8) | (str[2]<<16) | (str[3]<<24);
}


const uint32_t Task::MAGIC = make_magic_le("GCo1");
