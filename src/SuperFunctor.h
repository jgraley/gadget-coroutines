#ifndef SUPERFUNCTOR_H
#define SUPERFUNCTOR_H

#include <utility>
#include <cstddef>
#include <cstdint>

#define MAX_INSTRUCTIONS 4
#define SUPERFUNCTOR_TESTS

class SuperFunctor
{
public:
  SuperFunctor();
  virtual ~SuperFunctor();
  typedef void (*EntryPointFP)();
  EntryPointFP GetEntryPoint();

protected:  
  virtual void operator()();    

public:     // TODO private
  typedef void (SuperFunctor::* EntryPointMFP)();
  typedef void (*EntryPointFPT)( SuperFunctor *this_ );
  typedef uint16_t ThumbInstruction;
  
  static std::pair<ThumbInstruction *, int> GetAssembly();
  EntryPointFPT UnpackMemberFunctionPointer( EntryPointMFP mfp );

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