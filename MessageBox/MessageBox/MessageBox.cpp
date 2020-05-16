// MessageBox.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>

int messageBox() {
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


