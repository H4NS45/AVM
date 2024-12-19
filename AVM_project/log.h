#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>

#ifndef AVM_LOG
#define AVM_LOG(expr) avm::logging(expr)
#endif

namespace avm
{
	class log
	{
	public:
		log() {}
		~log()
		{
			fclose(m_fp);
		}
		FILE* m_fp = nullptr;
	};

	static log avmLog;

	static void logging(const char* szLog)
	{
		if (!avmLog.m_fp)
		{
			fopen_s(&avmLog.m_fp, "log.txt", "at");
		}
		SYSTEMTIME st;
		GetLocalTime(&st);
		fprintf(avmLog.m_fp, "MyLogInfo: %d:%d:%d:%d ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		fprintf(avmLog.m_fp, szLog);
		OutputDebugStringA(szLog);
	}

}