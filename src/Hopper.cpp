/**
 * @file Hopper.cpp
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 */

#include "Hopper.h"

#include <functional>
#include <atomic>

using namespace std;
using namespace HC;

Hopper::Hopper( std::function<void()> &&attach_, std::function<void()> &&detach_ ) :
  previous_hop( current_hop ),
  attach( move(attach_) ),
  detach( move(detach_) )
{
  if( previous_hop )
    previous_hop->detach();
  me()->set_hop_lambda( attach );
  current_hop = this;
}


void Hopper::hop(std::function<void()> &&new_attach, std::function<void()> &&new_detach)
{
  detach();
  attach = move(new_attach);
  detach = move(new_detach);
  me()->set_hop_lambda( attach );
}


Hopper::~Hopper()
{
  current_hop = previous_hop;
  detach();
  if( previous_hop )
    me()->set_hop_lambda( previous_hop->attach );
}


__thread Hopper *Hopper::current_hop;

