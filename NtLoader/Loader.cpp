#include <Windows.h>
#include <winternl.h>
#include <winnt.h>
#include <tchar.h>
#include <stdio.h>


typedef struct _LDR_MODULE {
    LIST_ENTRY              InLoadOrderModuleList;
    LIST_ENTRY              InMemoryOrderModuleList;
    LIST_ENTRY              InInitializationOrderModuleList;
    PVOID                   BaseAddress;
    PVOID                   EntryPoint;
    ULONG                   SizeOfImage;
    UNICODE_STRING          FullDllName;
    UNICODE_STRING          BaseDllName;
    ULONG                   Flags;
    SHORT                   LoadCount;
    SHORT                   TlsIndex;
    LIST_ENTRY              HashTableEntry;
    ULONG                   TimeDateStamp;

} LDR_MODULE, *PLDR_MODULE;



typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
    IN  HANDLE ProcessHandle,
    IN  PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN  ULONG ProcessInformationLength,
    OUT PULONG ReturnLength    OPTIONAL
    );

pfnNtQueryInformationProcess _pfnNtQueryInformationProcess = NULL;

BOOL LoadNtFunctions()
{
    BOOL ret = FALSE;
    HMODULE hNtDll = GetModuleHandle(TEXT("ntdll.dll"));
    if(hNtDll == NULL) return ret;

    _pfnNtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
    if(_pfnNtQueryInformationProcess == NULL)
    {
        FreeLibrary(hNtDll);
        return ret;
    }
    return !ret;
}



int main(int argc, char** argv)
{
	char c; 
	
	printf("What to do?\n[l]:load library\n[u]:unload library\n[q]: quit\n");
	while (1)
	{
		fflush(stdin);
		scanf_s("%c", &c, 1);

		HMODULE mLoadedModule = NULL;
		if (c == 'l')
		{
			mLoadedModule = LoadLibrary(TEXT("tracer.dll"));
		}
		else if (c == 'u')
		{
			if (mLoadedModule)
				FreeLibrary(mLoadedModule);
		}
		else if (c == 'q')
			break;
	}

	return 0;
}