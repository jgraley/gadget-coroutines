/**
 * @file SuperFunctor.h
 * ### `gadget-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 * 
 * @brief Permit a functor object to be invoked via C function pointer.
 */

#ifndef SuperFunctor_h
#define SuperFunctor_h

#include <utility>
#include <cstddef>
#include <cstdint>

#include "SuperFunctor_arm.h"

namespace GC
{

class SuperFunctor
{
public:
  SuperFunctor();
  virtual ~SuperFunctor() = default;
  typedef void (*EntryPointFP)();
  operator EntryPointFP();

protected:  
  virtual void operator()() = 0;    

private:
  typedef void (SuperFunctor::* EntryPointMFP)();
  typedef void (*EntryPointFPT)( SuperFunctor *this_ );
    
  static std::pair<Arm::MachineInstruction *, int> GetAssembly();

  Arm::MachineInstruction entrypoint_trampoline[SUPER_FUNCTOR_TRAMPOLINE_SIZE];
  EntryPointFPT entrypoint_fpt; 

#ifdef SUPERFUNCTOR_TESTS  
public:
  static void TestPCRelative();
  static void TestMemberFunctionPointer(); 
  static void TestUnpack(); 
#endif
};

} // namespace
#endif