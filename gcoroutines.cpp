  jmp_buf b;
  if(setjmp(b)==0)
  { 
    char *sp = (char *)gco::get_sp(b);
    char *fp = (char *)(__builtin_frame_address(0));
    bytes = fp-sp;
    char *nsp = s2+4096-bytes;
  
    memcpy( nsp, sp, bytes );
    gco::set_sp(b, nsp);
    
    longjmp(b, 123);
  }
# now in coroutine
