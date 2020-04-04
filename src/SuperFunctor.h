#ifndef SUPERFUNCTOR_H
#define SUPERFUNCTOR_H

class SuperFunctor
{
public:
  SuperFunctor();
  void operator()();
  static void TestPCRelative();
  static void TestMemberFunctionPointer();
  
private:    
  static void ReferenceAssmebly();
};

#endif