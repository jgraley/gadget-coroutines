#ifndef SUPERFUNCTOR_H
#define SUPERFUNCTOR_H

#include <utility>
#include <cstddef>
#include <cstdint>

#define MAX_INSTRUCTIONS 4

class SuperFunctor
{
public:
  SuperFunctor();
  virtual ~SuperFunctor();
  typedef void (*EntryPointFP)();
  operator EntryPointFP();

protected:  
  virtual void operator()() = 0;    

private:
  typedef void (SuperFunctor::* EntryPointMFP)();
  typedef void (*EntryPointFPT)( SuperFunctor *this_ );
  typedef uint16_t ThumbInstruction;
    
  static std::pair<ThumbInstruction *, int> GetAssembly();

  ThumbInstruction entrypoint_thunk[MAX_INSTRUCTIONS];
  EntryPointFPT entrypoint_fpt; 

#ifdef SUPERFUNCTOR_TESTS  
public:
  static void TestPCRelative();
  static void TestMemberFunctionPointer(); 
  static void TestUnpack(); 
#endif
};

#endif