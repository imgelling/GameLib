#pragma once
//#ifdef _DEBUG
//#include <intrin.h>
//#endif

#ifdef _DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#else
#define ASSERT(x)  
#endif