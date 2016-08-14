#include <Windows.h>
#include <winnt.h>
#include <tchar.h>
#include <stdio.h>


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