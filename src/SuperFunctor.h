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
  typedef void (SuperFunctor::* EntryPoint)();
  SuperFunctor();
  void SetEntrypoint( EntryPoint ep );
  virtual void operator()();    

public:     // TODO private
  typedef uint16_t ThumbInstruction;
  static std::pair<ThumbInstruction *, int> GetAssembly();
  ThumbInstruction entrypoint_thunk[MAX_INSTRUCTIONS];
  EntryPoint entrypoint_mfp; // TODO be a normal function pointer
};

#ifdef SUPERFUNCTOR_TESTS  
class TestClient : SuperFunctor
{
public:
  static void TestPCRelative();
  static void TestMemberFunctionPointer(); 
};
#endif

#endif