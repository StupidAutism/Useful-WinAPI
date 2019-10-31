#include <Windows.h>

#include "CSysInfo.h"

#include <stdio.h>

int main()
{
	CSysInfo* infmormer = new CSysInfo();

	PROC_LOAD procLoad;
	while (true)
	{
		CLEAR_CONSOLE();
		procLoad = infmormer->GetProcPerfInfo();
		for (int i = 0; i < procLoad.szCores; ++i)
		{
			wprintf(L"Core %i load: %f\n", i, procLoad.Usage[i]);
		}
		Sleep(1000);
	}

	delete infmormer;
	return 0;
}