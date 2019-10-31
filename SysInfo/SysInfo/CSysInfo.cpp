#include "CSysInfo.h"

#include <stdio.h>

CSysInfo::CSysInfo()
{
	NtQuerySystemInformation_ = (HRESULT(WINAPI*)(UINT, PVOID, ULONG, PULONG))GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQuerySystemInformation");
	if (!NtQuerySystemInformation_)
	{
		wprintf(L"Can't init NtQuerySystemInformation");
		ExitProcess(0x12345678);
	}

	sysInfo_ = {};
	dwCount_ = 0;
	GetSystemInfo(&sysInfo_);

	auto hHeap = GetProcessHeap();
	DWORD ProcNum = sysInfo_.dwNumberOfProcessors;

	Usage_		= (PFLOAT)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(FLOAT) * ProcNum);
	perfInfo_	= (PTRPERF_INFO)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(PERF_INFO) * ProcNum);
	idleTime_	= (PLARGE_INTEGER)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(LARGE_INTEGER) * ProcNum);

}

CSysInfo::~CSysInfo()
{
	auto hHeap = GetProcessHeap();
	HeapFree(hHeap, HEAP_ZERO_MEMORY, idleTime_);
	HeapFree(hHeap, HEAP_ZERO_MEMORY, perfInfo_);
	HeapFree(hHeap, HEAP_ZERO_MEMORY, Usage_);

}

PROC_LOAD CSysInfo::GetProcPerfInfo()
{
	PROC_LOAD RetVal{};
	RetVal.Usage = Usage_;
	RetVal.szCores = sysInfo_.dwNumberOfProcessors;

	// 8 specified for 'SystemPerformanceInformation' class 
	if (!NT_SUCCESS(NtQuerySystemInformation_(8, perfInfo_, sizeof(PERF_INFO) * (ULONG)sysInfo_.dwNumberOfProcessors, NULL)))
	{
		wprintf(L"NtQuerySystemInformation function failed");
		ExitProcess(0x87654321);
	}
	
	DWORD dwTickCount = GetTickCount();
	if (!dwCount_) dwCount_ = dwTickCount;

	// get cycle with counting deltas of CPU load
	for (DWORD i = 0; i < sysInfo_.dwNumberOfProcessors; i++)
	{
		FLOAT fTemp = 0.0f;
		PTRPERF_INFO cpuPerformanceInfo = &perfInfo_[i];
		cpuPerformanceInfo->KernelTime.QuadPart -= cpuPerformanceInfo->IdleTime.QuadPart;

		// to get info about load per CPU, we need to get by this
		Usage_[i] = 100.0f - 0.01f * (cpuPerformanceInfo->IdleTime.QuadPart - idleTime_[i].QuadPart) / ((dwTickCount - dwCount_));
		fTemp = Usage_[i];
		if (Usage_[i] < 0.0f) { Usage_[i] = 0.0f; }
		if (Usage_[i] > 100.0f) { Usage_[i] = 100.0f; }

		idleTime_[i] = cpuPerformanceInfo->IdleTime;
	}

	dwCount_ = dwTickCount;

	return RetVal;
}