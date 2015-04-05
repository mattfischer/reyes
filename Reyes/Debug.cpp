#include "Debug.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

void Debug::Print(const char *fmt, ...)
{
	char buf[1024];
	va_list v;
	va_start(v, fmt);

	vsprintf_s(buf, sizeof(buf), fmt, v);
	OutputDebugString(buf);
}