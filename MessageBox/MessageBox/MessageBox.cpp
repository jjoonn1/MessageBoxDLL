// MessageBox.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include "MessageBox.h"

double messageBox() {
	try {
		char szFileName[MAX_PATH];
		auto result = GetModuleFileNameA(0, szFileName, MAX_PATH);
		if (!result) {
			auto lastError = GetLastError();
			return lastError;
		}
		result = MessageBoxA(0, szFileName, "Success!", MB_ICONINFORMATION);
		if (!result) {
			auto lastError = GetLastError();
			return lastError;
		}
	}
	catch (...)
	{
		auto lastError = GetLastError();
		return lastError;
	}
}

bool IATHooking(HMODULE hInstance, LPCSTR targetFunction, PVOID newFunc)
{
	PIMAGE_THUNK_DATA pFirstThunk, pOriginalFirstThunk;
	PIMAGE_IMPORT_BY_NAME pFuncData;

	PIMAGE_IMPORT_DESCRIPTOR importedModule = getImportTable(hInstance);
	while (*(WORD*)importedModule != 0)
	{
		pFirstThunk = (PIMAGE_THUNK_DATA)((PBYTE)hInstance + importedModule->FirstThunk);
		pOriginalFirstThunk = (PIMAGE_THUNK_DATA)((PBYTE)hInstance + importedModule->OriginalFirstThunk);
		pFuncData = (PIMAGE_IMPORT_BY_NAME((PBYTE)hInstance + pOriginalFirstThunk->u1.AddressOfData));
		
		while (*(WORD*)pFirstThunk != 0 && *(WORD*)pOriginalFirstThunk != 0)
		{
			if (strcmp(targetFunction, (char*)pFuncData->Name) == 0)
			{
				SIZE_T bytesWritten = 0;
				DWORD oldProtect = 0;
				VirtualProtect((LPVOID)(&pFirstThunk->u1.Function), 8, PAGE_READWRITE, &oldProtect);

				// swap MessageBoxA address with address of hookedMessageBox
				pFirstThunk->u1.Function = (DWORD)newFunc;
			}
			pOriginalFirstThunk++;
			pFuncData = (PIMAGE_IMPORT_BY_NAME)((PBYTE)hInstance + pOriginalFirstThunk->u1.AddressOfData);
			pFirstThunk++;
		}
		importedModule++;
	}
	return true;
}

PIMAGE_IMPORT_DESCRIPTOR getImportTable(HMODULE hInstance)
{
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hInstance;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((PBYTE)dosHeader + dosHeader->e_lfanew);
	IMAGE_OPTIONAL_HEADER optionalHeader = ntHeader->OptionalHeader;
	IMAGE_DATA_DIRECTORY dataDirectory = (IMAGE_DATA_DIRECTORY)optionalHeader.DataDirectory[1];
	return (PIMAGE_IMPORT_DESCRIPTOR)((PBYTE)hInstance + dataDirectory.VirtualAddress);
}


