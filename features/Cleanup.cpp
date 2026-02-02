#include "Cleanup.h"

#include <Shlwapi.h>
#include <fstream>
#include <sstream>
#include <string>

#pragma comment(lib, "Shlwapi.lib")

bool Cleanup::GetExeDirectory(char* buffer, const size_t bufferSize)
{
	if (!GetModuleFileNameA(nullptr, buffer, static_cast<DWORD>(bufferSize)))
		return false;
	PathRemoveFileSpecA(buffer);
	return true;
}

void Cleanup::DeleteDumpFiles()
{
	char dllPath[MAX_PATH];
	if (!GetExeDirectory(dllPath, MAX_PATH))
		return;

	char searchPath[MAX_PATH];
	snprintf(searchPath, MAX_PATH, "%s\\*.dmp", dllPath);

	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(searchPath, &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do {
		char filePath[MAX_PATH];
		snprintf(filePath, MAX_PATH, "%s\\%s", dllPath, findData.cFileName);
		DeleteFileA(filePath);
	} while (FindNextFileA(hFind, &findData));

	FindClose(hFind);
}

void Cleanup::DeleteCrashSender()
{
	char dllPath[MAX_PATH];
	if (!GetExeDirectory(dllPath, MAX_PATH))
		return;

	char exePath[MAX_PATH];
	snprintf(exePath, MAX_PATH, "%s\\CrashSender.exe", dllPath);

	DeleteFileA(exePath);
}

void Cleanup::WipeUUID()
{
	char* docsPath = nullptr;
	size_t len = 0;
	if (_dupenv_s(&docsPath, &len, "USERPROFILE") != 0 || !docsPath)
		return;

	std::string iniPath = std::string(docsPath) + "\\Documents\\My Games\\Fallout 76\\Fallout76Prefs.ini";
	free(docsPath);

	std::ifstream inFile(iniPath);
	if (!inFile.is_open())
		return;

	std::ostringstream newContent;
	std::string line;
	bool inBethesdaSection = false;

	while (std::getline(inFile, line))
	{
		if (line == "[Bethesda.net]")
		{
			inBethesdaSection = true;
			continue;
		}

		if (inBethesdaSection)
		{
			if (!line.empty() && line[0] == '[')
			{
				inBethesdaSection = false;
				newContent << line << "\n";
			}
			else if (
				line.find("uPersistentUuidData0=") == 0 ||
				line.find("uPersistentUuidData1=") == 0 ||
				line.find("uPersistentUuidData2=") == 0 ||
				line.find("uPersistentUuidData3=") == 0)
			{
				continue;
			}
		}
		else
		{
			newContent << line << "\n";
		}
	}
	inFile.close();

	std::ofstream outFile(iniPath, std::ios::trunc);
	if (outFile.is_open())
	{
		outFile << newContent.str();
		outFile.close();
	}
}

void Cleanup::RunAll()
{
	DeleteDumpFiles();
	DeleteCrashSender();
	WipeUUID();
}
