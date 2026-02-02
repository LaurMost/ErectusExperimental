#pragma once
#include <string>
#include <vector>
#include "Windows.h"

class ErectusProcess final {
public:
    static void ResetProcessData();
    static bool AttachToProcess(DWORD processId);
    static void SetProcessError(int errorId, const char* error);
    static std::vector<DWORD> GetProcesses();
    static bool HwndValid(DWORD processId);

    inline static std::string processError;
    inline static int processErrorId = 0;
    inline static HANDLE handle = nullptr;
    inline static HWND hWnd = nullptr;
    inline static std::uintptr_t exe = 0;
    inline static DWORD pid = 0;

    // Memory operations
    static bool Rpm(std::uintptr_t src, void* dst, size_t size);
    static bool Wpm(std::uintptr_t dst, const void* src, size_t size);

    // Stealth allocation API (RW → RX pattern)
    static std::uintptr_t AllocEx(size_t size);                              // Allocate RW
    static bool ProtectEx(std::uintptr_t addr, size_t size);                 // RW → RX
    static std::uintptr_t AllocWriteProtect(const void* data, size_t size);  // Alloc + Write + Protect (one-shot)
    static bool ReprotectWrite(std::uintptr_t addr, const void* data, size_t size);  // RX → RW → Write → RX (updates)
    static bool FreeEx(std::uintptr_t src);

private:
    static BOOL HwndEnumFunc(HWND hwnd, LPARAM lParam);
    static std::uintptr_t GetModuleBaseAddress(DWORD procId, const char* module);
    virtual void Dummy() = 0;
};
