#define _CRT_SECURE_NO_WARNINGS

#include <conio.h>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>
const int kDelay = 100;

using namespace std;

int main()
{
	HANDLE Work = OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,	TEXT("Work"));
	HANDLE Close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("Close"));
	HANDLE WINAPI FileProjection = OpenFileMapping(	FILE_MAP_ALL_ACCESS,	FALSE,	TEXT("FileProjection"));
	LPVOID pointer;
	pointer = MapViewOfFile(FileProjection,FILE_MAP_ALL_ACCESS,	0,	0,	0);
	char* stringRepresentation = (char*)pointer;

	while (true)
	{
		WaitForSingleObject(Work, INFINITE);
		if (WaitForSingleObject(Close, kDelay) == WAIT_OBJECT_0) {
			CloseHandle(Work);
			CloseHandle(Close);
			UnmapViewOfFile(pointer);
			CloseHandle(FileProjection);
			return 0;
		}
		cout << "Client got: " << stringRepresentation << endl;
		ReleaseSemaphore(Work, 1, NULL);
	}
}