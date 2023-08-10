#pragma once

#ifdef _DEBUG
#include<windows.h>
#define WARNING(flag,msg1,msg2)\
  { if(flag){\
      char buf[ 512 ];\
      wsprintfA( buf, "%s\n%s\n\n%s : %d", msg1, msg2, __FILE__, __LINE__ );\
      MessageBoxA( 0, buf, "WARNING!!", MB_OK | MB_ICONERROR );\
      exit(1);\
    }\
  }
#else
  #define WARNING(flag,msg1,msg2) {}
#endif

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
