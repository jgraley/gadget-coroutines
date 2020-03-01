#include <csetjmp>

namespace gco
{

#if defined(__arm__) || defined(__thumb__)
static const int ARM_JMPBUF_INDEX_SP = 8;
inline void *get_sp( jmp_buf env )
{
    return static_cast<void *>( env[ARM_JMPBUF_INDEX_SP] );
}
inline void set_sp( jmp_buf env, void *new_sp )
{
    env[ARM_JMPBUF_INDEX_SP] = static_cast<int>(new_sp);
}
#endif

};
