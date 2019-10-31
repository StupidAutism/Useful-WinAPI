# Useful-WinAPI
Useful and handful classes and functions using C++ and WinAPI  
## Projects:
### SysInfo:
Helper class that helps to retrieve cpu usage per core.  
Some updates to retrieve gpu, ram and disk usage will be delivered later.
### Coming soon...

***
## Usage:
### SysInfo:
```cpp
#include "CSysInfo.h"
#include <stdio.h>

int main()
{
    CSysInfo* informer = new CSysInfo();
    PROC_LOAD procLoad;
    while(true)
    {
    	// this macro defined in 'CSysInfo.h' file
    	CLEAR_CONSOLE();
        procLoad = informer->GetProcLoadInfo();
        for (int i = 0; i < procLoad.szCores; ++i)
            wprintf(L"Core %i load: %f\n", i, procLoad.Usage[i]);
        Sleep(1000);
    }
}

```