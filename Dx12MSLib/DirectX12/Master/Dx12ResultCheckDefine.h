#pragma once
#ifdef _DEBUG
#include <cassert>

#define D12RESULTCHECK(result,dev) if(FAILED(result)) { \
	result = dev->GetDeviceRemovedReason();	\
	assert(false);	\
}
#else
#define D12RESULTCHECK(result,dev)

#endif