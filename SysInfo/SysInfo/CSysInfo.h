#ifndef CSYSINFO_H
#define CSYSINFO_H

#include <Windows.h>


// NT specific defines
#define NT_SUCCESS(Status)		(((NTSTATUS)(Status)) >= 0)
#define NT_INFORMATION(Status)	((((ULONG)(Status)) >> 30) == 1)
#define NT_WARNING(Status)		((((ULONG)(Status)) >> 30) == 2)
#define NT_ERROR(Status)		((((ULONG)(Status)) >> 30) == 3)

// to clear console
#define CLEAR_CONSOLE() system("CLS")

typedef HRESULT(WINAPI* NtQuerySystemInformationType)(UINT, PVOID, ULONG, PULONG);

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
{
	LARGE_INTEGER IdleTime;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER DpcTime;
	LARGE_INTEGER InterruptTime;
	ULONG InterruptCount;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, * PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

// shorter copy of prev type
typedef SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION PERF_INFO, * PTRPERF_INFO;

typedef struct  
{
	PFLOAT Usage;
	UINT szCores;
} PROC_LOAD;

class CSysInfo
{
	SYSTEM_INFO		sysInfo_;
	PFLOAT			Usage_;
	PTRPERF_INFO	perfInfo_;
	PLARGE_INTEGER	idleTime_;
	DWORD			dwCount_;

	NtQuerySystemInformationType NtQuerySystemInformation_;
public:
	CSysInfo();
	~CSysInfo();

	PROC_LOAD GetProcPerfInfo();

};


#endif // CSYSINFO_H