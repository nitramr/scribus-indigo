// scripter.cpp : d�finit le point d'entr�e pour l'application DLL.
//

#define WIN32_LEAN_AND_MEAN		// Exclure les en-t�tes Windows rarement utilis�s
// Fichiers d'en-t�te Windows�:
#include <windows.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

