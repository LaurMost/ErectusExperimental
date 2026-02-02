#pragma once

#include <Windows.h>

class Cleanup final {
public:
	static void DeleteDumpFiles();
	static void DeleteCrashSender();
	static void WipeUUID();
	static void RunAll();

private:
	static bool GetExeDirectory(char* buffer, size_t bufferSize);

	virtual void Dummy() = 0;
};
