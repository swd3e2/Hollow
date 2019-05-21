#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

using UINT = unsigned int;
