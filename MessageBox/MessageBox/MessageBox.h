#pragma once
double messageBox();
bool IATHooking(HMODULE hInstance, LPCSTR targetFunction, PVOID newFunc);
PIMAGE_IMPORT_DESCRIPTOR getImportTable(HMODULE hInstance);