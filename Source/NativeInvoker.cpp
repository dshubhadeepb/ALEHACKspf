#include "stdafx.h"

static NativeManagerContext g_context;
static UINT64 g_hash;
void(*scrNativeCallContext::SetVectorResults)(scrNativeCallContext*) = nullptr;

void nativeInit(UINT64 hash) 
{
	g_context.Reset();
	g_hash = hash;
}

void nativePush64(UINT64 value) 
{
	g_context.Push(value);
}

uint64_t * nativeCall() 
{
	auto fn = GameHooking::GetNativeHandler(g_hash);
	if (fn != 0) 
	{
		static void* exceptionAddress;
		__try
		{
			fn(&g_context);
			scrNativeCallContext::SetVectorResults(&g_context);
		}
		__except (exceptionAddress = (GetExceptionInformation())->ExceptionRecord->ExceptionAddress, EXCEPTION_EXECUTE_HANDLER)
		{
			std::cout << xorstr_("Failed to execute Game Function ") << exceptionAddress << std::endl;
		}
	}
	return reinterpret_cast<uint64_t*>(g_context.GetResultPointer());
}

