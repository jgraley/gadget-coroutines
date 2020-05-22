#ifndef SUPERFUNCTOR_H
#define SUPERFUNCTOR_H

#include <utility>
#include <cstddef>
#include <cstdint>

#include "SuperFunctor_arm.h"

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
    
  static std::pair<MachineInstruction *, int> GetAssembly();

  MachineInstruction entrypoint_thunk[SUPER_FUNCTOR_THUNK_ASSEMBLY_SIZE];
  EntryPointFPT entrypoint_fpt; 

#ifdef SUPERFUNCTOR_TESTS  
public:
  static void TestPCRelative();
  static void TestMemberFunctionPointer(); 
  static void TestUnpack(); 
#endif
};

#endif